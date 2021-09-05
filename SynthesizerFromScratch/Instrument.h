#pragma once

namespace SynthesizerFromScratch
{
	class Instrument
	{
	public:
		virtual double playSound(double dTime, double dFrequency) = 0;
		
		double dVolume;
	};

	class Bell : Instrument
	{
	public:
		Bell();

		double playSound(double dTime, double dFrequency) override;
	};

	class Harmonica
	{
	public:
		Harmonica();

		static double playSound(double dTime, double dFrequency);
	};
}