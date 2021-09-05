#include "Synth.h"

SynthesizerFromScratch::Synth::Synth()
{
	olcNoiseMaker<short>* soundMachine = createSoundMachine();
	soundMachine->SetUserFunction(makeNoise);
}

olcNoiseMaker<short>* SynthesizerFromScratch::Synth::createSoundMachine()
{
	vector<wstring> devices = olcNoiseMaker<short>::Enumerate();

	for (auto d : devices)
		std::wcout << "Found Output Device: " << d << std::endl;
	std::wcout << "Selecting: " << devices[0] << std::endl;

	olcNoiseMaker<short> *soundMachine = new olcNoiseMaker<short>(devices[0], 44100, 1, 8, 512);

	return soundMachine;
}

double SynthesizerFromScratch::Synth::makeNoise(double dTime)
{
	return Harmonica::playSound(dTime, 880.0);
}