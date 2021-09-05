#pragma once

namespace SynthesizerFromScratch {

	struct Oscillator
	{
		enum class oscType
		{
			SIN,
			SQUARE,
			TRIANGLE,
			SAW_ANALOGUE,
			SAW,
			NOISE,
		};

		static double getValue(oscType type, double dFrequency, double dTime, double dLFOFrequency = 0.0, double dLFOAmplitude = 0.0);

	private:
		static double frequencyToW(double dFrequency);
		static double sampleSinWave(double dFrequency, double dTime);
		static double sampleSquareWave(double dFrequency, double dTime);
		static double sampleTriangleWave(double dFrequency, double dTime);
		static double sampleSawAnalogueWave(double dFrequency, double dTime);
		static double sampleSawWave(double dFrequency, double dTime);
		static double sampleNoise();
	};

}