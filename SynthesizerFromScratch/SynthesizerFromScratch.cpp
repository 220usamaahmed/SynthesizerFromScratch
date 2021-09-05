#include <iostream>
#include "Synth.h"

int main()
{
	std::cout << "--- Synthesizer From Scratch ---" << std::endl;

	SynthesizerFromScratch::Synth synth;

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
					// SynthesizerFromScratch::Synth::dCurrentFrequency = dFrequencyOutput;
					nCurrentKey = k;
				}
				bKeyPressed = true;
			}
		}

		if (!bKeyPressed)
		{
			if (nCurrentKey != -1)
			{
				nCurrentKey = -1;
			}
		}
	}
}
