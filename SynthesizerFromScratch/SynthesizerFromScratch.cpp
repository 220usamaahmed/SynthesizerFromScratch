#include <iostream>
#include "olcNoiseMaker.h";

#define PI 3.14159

atomic<double> dFrequencyOutput = 0.0;

double makeSinWave(double dtime)
{
    return 0.5 * sin(dFrequencyOutput * 2 * PI * dtime);
}

double makeSquareWave(double dtime)
{
    if (sin(dFrequencyOutput * 2 * PI * dtime) > 0) return 0.1;
    else return -0.1;
}

int main()
{
    // Get all sound hardware
    vector<wstring> devices = olcNoiseMaker<short>::Enumerate();
    for (auto d : devices) wcout << "Found Output Device: " << d << endl;

    // Create sound machine
    olcNoiseMaker<short> sound(devices[0], 44100, 1, 8, 512);

    // Link noise function with sound machine
    sound.SetUserFunction(makeSquareWave);

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
