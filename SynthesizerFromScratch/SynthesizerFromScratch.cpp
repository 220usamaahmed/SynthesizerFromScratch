#include <iostream>
#include "olcNoiseMaker.h";

struct sEnvelopeADSR
{
    double dAttackTime;
    double dDecayTime;
    double dReleaseTime;

    double dSustainAmplitude;
    double dStartAmplitude;

    double dTriggerOnTime;
    double dTriggerOffTime;

    bool bNoteOn;

    sEnvelopeADSR()
    {
        dAttackTime = 0.1;
        dDecayTime = 0.01;
        dStartAmplitude = 1.0;
        dSustainAmplitude = 0.8;
        dReleaseTime = 0.2;
        dTriggerOnTime = 0.0;
        dTriggerOffTime = 0.0;
        bNoteOn = false;
    }

    void NoteOn(double dTimeOn)
    {
        dTriggerOnTime = dTimeOn;
        bNoteOn = true;
    }

    void NoteOff(double dTimeOff)
    {
        dTriggerOffTime = dTimeOff;
        bNoteOn = false;
    }

    double GetAmplitude(double dTime)
    {
        double dAmplitude = 0.0;

        double dLifeTime = dTime - dTriggerOnTime;

        if (bNoteOn)
        {
            // ADS handled here
            
            // Attach
            if (dLifeTime <= dAttackTime)
                dAmplitude = (dLifeTime / dAttackTime) * dStartAmplitude;

            // Decay
            if (dLifeTime > dAttackTime && dLifeTime <= (dAttackTime + dDecayTime))
                dAmplitude = ((dLifeTime - dAttackTime) / dDecayTime)
                    * (dSustainAmplitude - dStartAmplitude) 
                    + dStartAmplitude;

            // Sustain
            if (dLifeTime > (dAttackTime + dDecayTime))
                dAmplitude = dSustainAmplitude;
        }
        else
        {
            // R handled here
            dAmplitude = ((dTime - dTriggerOffTime) / dReleaseTime) * (0.0 - dSustainAmplitude) + dSustainAmplitude;
        }

        if (dAmplitude <= 0.0001)
            dAmplitude = 0.0;

        return dAmplitude;
    }
};

atomic<double> dFrequencyOutput = 0.0;

enum class oscType
{
    SIN,
    SQUARE,
    TRIANGLE,
    SAW_ANALOGUE,
    SAW
};

double dOctaveBaseFrequency = 110.0;
double d12thRootOf2 = pow(2.0, 1.0 / 12);
double minorThird = pow(2.0, 3.0 / 12);
double majorThird = pow(2.0, 4.0 / 12);
double perfectFifth = pow(2.0, 7.0 / 12);
sEnvelopeADSR envelope;

double w(double dHertz)
{
    return dHertz * 2 * PI;
}

double sampleSinWave(double dHertz, double dTime)
{
    return 0.5 * sin(w(dHertz) * dTime);
}

double sampleSquareWave(double dHertz, double dTime)
{
    return (sin(w(dHertz) * dTime) > 0) ? 0.1 : -0.1;
}

double sampleTriangleWave(double dHertz, double dTime)
{
    return asin(sin(w(dHertz) * dTime) * 2.0 / PI);
}

double sampleSawAnalogueWave(double dHertz, double dTime)
{
    double dOutput = 0.0;

    for (double n = 1.0; n < 4; ++n)
        dOutput += (sin(n * w(dHertz) * dTime)) / n;
    
    return dOutput * (2.0 / PI);
}

double sampleSawWave(double dHertz, double dTime)
{
    return (2.0 / PI) * (dHertz * PI * fmod(dTime, 1.0 / dHertz) - (PI / 2.0));
}


double osc(double dHertz, double dTime, oscType type)
{
    switch (type)
    {
    case oscType::SIN:
        return sampleSinWave(dHertz, dTime);
    case oscType::SQUARE:
        return sampleSquareWave(dHertz, dTime);
    case oscType::TRIANGLE:
        return sampleTriangleWave(dHertz, dTime);
    case oscType::SAW_ANALOGUE:
        return sampleSawAnalogueWave(dHertz, dTime);
    case oscType::SAW:
        return sampleSawWave(dHertz, dTime);
    default:
        return 0.0;
    }
}

double playMajorChord(double dTime)
{
    return envelope.GetAmplitude(dTime) *
        (
            // Major chord
            +osc(dFrequencyOutput, dTime, oscType::SAW_ANALOGUE)
            + osc(dFrequencyOutput * majorThird, dTime, oscType::SAW_ANALOGUE)
            + osc(dFrequencyOutput * perfectFifth, dTime, oscType::SAW_ANALOGUE)
            ) * 0.4;
}

double playMinorChord(double dTime)
{
    return envelope.GetAmplitude(dTime) *
        (
            // Major chord
            +osc(dFrequencyOutput, dTime, oscType::SAW_ANALOGUE)
            + osc(dFrequencyOutput * minorThird, dTime, oscType::SAW_ANALOGUE)
            + osc(dFrequencyOutput * perfectFifth, dTime, oscType::SAW_ANALOGUE)
            ) * 0.4;
}

int main()
{
    // Get all sound hardware
    vector<wstring> devices = olcNoiseMaker<short>::Enumerate();
    for (auto d : devices) wcout << "Found Output Device: " << d << endl;

    // Create sound machine
    olcNoiseMaker<short> sound(devices[0], 44100, 1, 8, 512);

    // Link noise function with sound machine
    // sound.SetUserFunction(playMajorChord);
    sound.SetUserFunction(playMinorChord);

    int nCurrentKey = -1;
    bool bKeyPressed = false;

    while (1)
    {
        bKeyPressed = false;
        for (int k = 0; k < 16; k++)
        {
            if (GetAsyncKeyState((unsigned char)("ZSXCFVGBNJMK\xbcL\xbe\xbf "[k])) & 0x8000)
            {
                if (nCurrentKey != k)
                {
                    dFrequencyOutput = dOctaveBaseFrequency * pow(d12thRootOf2, k);
                    envelope.NoteOn(sound.GetTime());
                    nCurrentKey = k;
                }
                bKeyPressed = true;
            }
        }

        if (!bKeyPressed)
        {
            if (nCurrentKey != -1)
            {
                envelope.NoteOff(sound.GetTime());
                nCurrentKey = -1;
            }
        }
    }

    return 0;
}
