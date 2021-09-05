#pragma once

namespace SynthesizerFromScratch
{
	class Instrument
	{
	public:
		virtual double playSound(double dTime, double dFrequency);
		
		double dVolume;
	};

	class Bell : public Instrument
	{
	public:
		Bell();

		double playSound(double dTime, double dFrequency) override;
	};

	class Harmonica : public Instrument
	{
	public:
		Harmonica();

		double playSound(double dTime, double dFrequency) override;
	};
}