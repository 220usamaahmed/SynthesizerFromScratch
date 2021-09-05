#include "Instrument.h"
#include "Oscillator.h"

using namespace SynthesizerFromScratch;

Bell::Bell()
{
	dVolume = 0.4;
}

double Bell::playSound(double dTime, double dFrequency)
{
	return (
		+ 1.0 * Oscillator::getValue(Oscillator::oscType::SIN, dFrequency * 2.0, dTime, 5.0, 0.001)
		+ 0.5 * Oscillator::getValue(Oscillator::oscType::SIN, dFrequency * 3.0, dTime)
		+ 0.25 * Oscillator::getValue(Oscillator::oscType::SIN, dFrequency * 4.0, dTime)
		) * dVolume;
}

Harmonica::Harmonica()
{
	
}

double Harmonica::playSound(double dTime, double dFrequency)
{
	return (
		+1.0 * Oscillator::getValue(Oscillator::oscType::SQUARE, dFrequency * 2.0, dTime, 5.0, 0.001)
		+ 0.5 * Oscillator::getValue(Oscillator::oscType::SQUARE, dFrequency * 3.0, dTime)
		+ 0.25 * Oscillator::getValue(Oscillator::oscType::NOISE, dFrequency * 4.0, dTime)
		) * 0.4;
}