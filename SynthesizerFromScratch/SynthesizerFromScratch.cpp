#include <iostream>
#include "olcNoiseMaker.h";

atomic<double> dFrequencyOutput = 0.0;

enum class oscType
{
    SIN,
    SQUARE,
    TRIANGLE,
    SAW_ANALOGUE,
    SAW
};

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

    for (double n = 1.0; n < 10; ++n)
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

double makeNoise(double dTime)
{
    return osc(dFrequencyOutput, dTime, oscType::SAW_ANALOGUE) * 0.4;
}

int main()
{
    // Get all sound hardware
    vector<wstring> devices = olcNoiseMaker<short>::Enumerate();
    for (auto d : devices) wcout << "Found Output Device: " << d << endl;

    // Create sound machine
    olcNoiseMaker<short> sound(devices[0], 44100, 1, 8, 512);

    // Link noise function with sound machine
    sound.SetUserFunction(makeNoise);

    double dOctaveBaseFrequency = 110.0;
    double d12thRootOf2 = pow(2.0, 1.0 / 12);

    while (1)
    {
        bool bKeyPressed = false;
        for (int k = 0; k < 15; k++)
        {
            if (GetAsyncKeyState((unsigned char)("ZSXCFVGBNJMK\xbcL\xbc"[k])) & 0x8000)
            {
                dFrequencyOutput = dOctaveBaseFrequency * pow(d12thRootOf2, k);
                bKeyPressed = true;
            }
        }

        if (!bKeyPressed)
        {
            dFrequencyOutput = 0.0;
        }
    }

    return 0;
}
