#include "Instrument.h"
#include "Oscillator.h"

Bell::Bell()
{
	dVolume = 0.4;
	env.dAttackTime = 0.01;
	env.dDecayTime = 0.8;
	env.dSustainAmplitude = 0.0;
	env.dReleaseTime = 1.0;
}

double Bell::playSound(double dFrequency, double dTime, double startTime, double endTime, bool &bNoteFinished)
{
	double dAmplitude = env.amplitude(dTime, startTime, endTime);
	if (dAmplitude <= 0.0) bNoteFinished = false;

	return dAmplitude * (
		+ 1.0 * Oscillator::getValue(Oscillator::oscType::SIN, dFrequency * 2.0, dTime, 5.0, 0.001)
		+ 0.5 * Oscillator::getValue(Oscillator::oscType::SIN, dFrequency * 3.0, dTime)
		+ 0.25 * Oscillator::getValue(Oscillator::oscType::SIN, dFrequency * 4.0, dTime)
		) * dVolume;
}

Harmonica::Harmonica()
{
	dVolume = 0.4;
	env.dAttackTime = 0.00;
	env.dDecayTime = 1.0;
	env.dSustainAmplitude = 0.95;
	env.dReleaseTime = 0.1;
}

double Harmonica::playSound(double dFrequency, double dTime, double startTime, double endTime, bool& bNoteFinished)
{
	double dAmplitude = env.amplitude(dTime, startTime, endTime);
	if (dAmplitude <= 0.0) bNoteFinished = false;

	return dAmplitude * (
		+1.0 * Oscillator::getValue(Oscillator::oscType::SQUARE, dFrequency * 2.0, dTime, 5.0, 0.001)
		+ 0.5 * Oscillator::getValue(Oscillator::oscType::SQUARE, dFrequency * 3.0, dTime)
		+ 0.25 * Oscillator::getValue(Oscillator::oscType::NOISE, dFrequency * 4.0, dTime)
		) * dVolume;
}