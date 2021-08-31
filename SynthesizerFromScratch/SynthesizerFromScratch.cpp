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
    SAW,
    NOISE
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
    return 0.5 * sin(dHertz);
}

double sampleSquareWave(double dHertz, double dTime)
{
    return (sin(dHertz) > 0) ? 0.1 : -0.1;
}

double sampleTriangleWave(double dHertz, double dTime)
{
    return asin(sin(dHertz) * 2.0 / PI);
}

double sampleSawAnalogueWave(double dHertz, double dTime)
{
    double dOutput = 0.0;

    for (double n = 1.0; n < 4; ++n)
        dOutput += (sin(n * dHertz)) / n;
    
    return dOutput * (2.0 / PI);
}

double sampleSawWave(double dHertz, double dTime)
{
    return (2.0 / PI) * (dHertz * PI * fmod(dTime, 1.0 / dHertz) - (PI / 2.0));
}

double sampleNoise()
{
    return 2.0 * ((double)rand() / (double)RAND_MAX) - 1.0;
}

double osc(double dHertz, double dTime, oscType type, double dLFOHertz = 0.0, double dLFOAmplitude = 0.0)
{
    double dFreq = w(dHertz) * dTime + dLFOAmplitude * dHertz * sin(w(dLFOHertz) * dTime);

    switch (type)
    {
    case oscType::SIN:
        return sampleSinWave(dFreq, dTime);
    case oscType::SQUARE:
        return sampleSquareWave(dFreq, dTime);
    case oscType::TRIANGLE:
        return sampleTriangleWave(dFreq, dTime);
    case oscType::SAW_ANALOGUE:
        return sampleSawAnalogueWave(dFreq, dTime);
    case oscType::SAW:
        return sampleSawWave(dHertz, dTime);
    case oscType::NOISE:
        return sampleNoise();
    default:
        return 0.0;
    }
}

struct Instrument
{
    double dVolume;
    sEnvelopeADSR env;

    virtual double sound(double dTime, double dFrequency) = 0;
};

struct Bell : Instrument
{
    Bell()
    {
        dVolume = 0.4;

        env.dAttackTime = 0.01;
        env.dDecayTime = 1.0;
        env.dStartAmplitude = 1.0;
        env.dSustainAmplitude = 0.0;
        env.dReleaseTime = 1.0;
    }

    double sound(double dTime, double dFrequency)
    {
        return env.GetAmplitude(dTime) *
            (
                + 1.0 * osc(dFrequencyOutput * 2.0, dTime, oscType::SIN, 5.0, 0.001)
                + 0.5 * osc(dFrequencyOutput * 3.0, dTime, oscType::SIN)
                + 0.25 * osc(dFrequencyOutput * 4.0, dTime, oscType::SIN)
                ) * dVolume;
    }
};

struct Harmonica : Instrument
{
    Harmonica()
    {
        dVolume = 0.4;

        env.dAttackTime = 0.05;
        env.dDecayTime = 1.0;
        env.dStartAmplitude = 1.0;
        env.dSustainAmplitude = 0.95;
        env.dReleaseTime = 0.1;
    }

    double sound(double dTime, double dFrequency)
    {
        return env.GetAmplitude(dTime) *
            (
                + 1.0 * osc(dFrequencyOutput * 2.0, dTime, oscType::SQUARE, 5.0, 0.001)
                + 0.5 * osc(dFrequencyOutput * 3.0, dTime, oscType::SQUARE)
                + 0.25 * osc(dFrequencyOutput * 4.0, dTime, oscType::NOISE)
                ) * dVolume;
    }
};

Instrument* voice = nullptr;

double playMajorChord(double dTime)
{
    return envelope.GetAmplitude(dTime) *
        (
            // Major chord
            + osc(dFrequencyOutput, dTime, oscType::SAW_ANALOGUE)
            + osc(dFrequencyOutput * majorThird, dTime, oscType::SAW_ANALOGUE)
            + osc(dFrequencyOutput * perfectFifth, dTime, oscType::SAW_ANALOGUE)
            ) * 0.4;
}

double playMinorChord(double dTime)
{
    return envelope.GetAmplitude(dTime) *
        (
            // Major chord
            + osc(dFrequencyOutput, dTime, oscType::SAW_ANALOGUE)
            + osc(dFrequencyOutput * minorThird, dTime, oscType::SAW_ANALOGUE)
            + osc(dFrequencyOutput * perfectFifth, dTime, oscType::SAW_ANALOGUE)
            ) * 0.4;
}

double makeNoise(double dTime)
{
    return voice->sound(dTime, dFrequencyOutput);
}

int main()
{
    // Get all sound hardware
    vector<wstring> devices = olcNoiseMaker<short>::Enumerate();
    for (auto d : devices) wcout << "Found Output Device: " << d << endl;

    // Create sound machine
    olcNoiseMaker<short> sound(devices[0], 44100, 1, 8, 512);

    voice = new Harmonica();

    // Link noise function with sound machine
    // sound.SetUserFunction(playMajorChord);
    sound.SetUserFunction(makeNoise);

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
                    voice->env.NoteOn(sound.GetTime());
                    nCurrentKey = k;
                }
                bKeyPressed = true;
            }
        }

        if (!bKeyPressed)
        {
            if (nCurrentKey != -1)
            {
                voice->env.NoteOff(sound.GetTime());
                nCurrentKey = -1;
            }
        }
    }

    return 0;
}
