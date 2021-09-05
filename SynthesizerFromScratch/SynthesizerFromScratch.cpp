#include <iostream>
#include "olcNoiseMaker.h"
#include "Instrument.h"

SynthesizerFromScratch::Instrument* instrument = new SynthesizerFromScratch::Bell();
double dCurrentFrequency = 0.0;

double userFunction(double dTime)
{
	return instrument->playSound(dTime, dCurrentFrequency);
}

void initSynth()
{
	vector<wstring> devices = olcNoiseMaker<short>::Enumerate();

	for (auto d : devices)
		std::wcout << "Found Output Device: " << d << std::endl;
	std::wcout << "Selecting: " << devices[0] << std::endl;

	olcNoiseMaker<short>* soundMachine = new olcNoiseMaker<short>(devices[0], 44100, 1, 8, 512);

	soundMachine->SetUserFunction(userFunction);
}

int main()
{
	std::cout << "--- Synthesizer From Scratch ---" << std::endl;

	initSynth();

	int nCurrentKey = -1;
	bool bKeyPressed = false;
	double dFrequencyOutput;
	double dOctaveBaseFrequency = 110.0;
	double d12thRootOf2 = pow(2.0, 1.0 / 12);

	while (1)
	{
		bKeyPressed = false;
		for (int k = 0; k < 16; ++k)
		{
			if (GetAsyncKeyState((unsigned char)("ZSXCFVGBNJMK\xbcL\xbe\xbf "[k])) & 0x8000)
			{
				if (nCurrentKey != k)
				{
					dFrequencyOutput = dOctaveBaseFrequency * pow(d12thRootOf2, k);
					dCurrentFrequency = dFrequencyOutput;
					nCurrentKey = k;
				}
				bKeyPressed = true;
			}
		}

		if (!bKeyPressed)
		{
			if (nCurrentKey != -1)
			{
				dCurrentFrequency = 0.0;
				nCurrentKey = -1;
			}
		}
	}
}
