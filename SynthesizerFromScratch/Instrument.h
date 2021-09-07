#pragma once

struct Envelope
{
	double dAttackTime;
	double dDecayTime;
	double dSustainAmplitude;
	double dStartAmplitude;
	double dReleaseTime;

	Envelope()
	{
		dAttackTime = 0.1;
		dDecayTime = 0.1;
		dSustainAmplitude = 1.0;
		dReleaseTime = 0.2;
		dStartAmplitude = 1.0;
	}

	double amplitude(const double dTime, const double dTimeOn, const double dTimeOff)
	{
		double dAmplitude = 0.0;
		double dReleaseAmplitude = 0.0;

		if (dTimeOn > dTimeOff)
		{
			double dLifeTime = dTime - dTimeOn;

			// Attack Phase
			if (dLifeTime <= dAttackTime)
				dAmplitude = (dLifeTime / dAttackTime) * dStartAmplitude;

			// Decay Phase
			if (dLifeTime > dAttackTime && dLifeTime <= (dAttackTime + dDecayTime))
				dAmplitude = ((dLifeTime - dAttackTime) / dDecayTime) * (dSustainAmplitude - dStartAmplitude) + dStartAmplitude;

			// Sustain Phase
			if (dLifeTime > (dAttackTime + dDecayTime))
				dAmplitude = dSustainAmplitude;
		}
		else
		{
			double dLifeTime = dTimeOff - dTimeOn;

			// Offed during attack phase
			if (dLifeTime <= dAttackTime)
				dReleaseAmplitude = (dLifeTime / dAttackTime) * dStartAmplitude;

			// Offed during decay phase
			if (dLifeTime > dAttackTime && dLifeTime <= (dAttackTime + dDecayTime))
				dReleaseAmplitude = ((dLifeTime - dAttackTime) / dDecayTime) * (dSustainAmplitude - dStartAmplitude) + dStartAmplitude;

			// Offed during sustain phase
			if (dLifeTime > (dAttackTime + dDecayTime))
				dReleaseAmplitude = dSustainAmplitude;

			dAmplitude = ((dTime - dTimeOff) / dReleaseTime) * (0.0 - dReleaseAmplitude) + dReleaseAmplitude;
		}

		if (dAmplitude <= 0.01)
			dAmplitude = 0.0;

		return dAmplitude;
	}
};

class Instrument
{
public:
	virtual double playSound(double dFrequency, double dTime, double startTime, double endTime, bool& bNoteFinished) = 0;
		
	double dVolume;
	Envelope env;
};

class Bell : public Instrument
{
public:
	Bell();

	double playSound(double dFrequency, double dTime, double startTime, double endTime, bool& bNoteFinished);
};

class Harmonica : public Instrument
{
public:
	Harmonica();

	double playSound(double dFrequency, double dTime, double startTime, double endTime, bool& bNoteFinished);
};