#include "Oscillator.h"
#include <cmath>

double SynthesizerFromScratch::Oscillator::getValue(oscType type, double dFrequency, double dTime, double dLFOFrequency, double dLFOAmplitude)
{
    double dFreqencyWithLFO = frequencyToW(dFrequency) * dTime + dLFOAmplitude * dFrequency * sin(frequencyToW(dLFOFrequency) * dTime);

    switch (type)
    {
    case oscType::SIN:
        return sampleSinWave(dFreqencyWithLFO, dTime);
    case oscType::SQUARE:
        return sampleSquareWave(dFreqencyWithLFO, dTime);
    case oscType::TRIANGLE:
        return sampleTriangleWave(dFreqencyWithLFO, dTime);
    case oscType::SAW_ANALOGUE:
        return sampleSawAnalogueWave(dFreqencyWithLFO, dTime);
    case oscType::SAW:
        return sampleSawWave(dFrequency, dTime);
    case oscType::NOISE:
        return sampleNoise();
    default:
        return 0.0;
    }
}

double SynthesizerFromScratch::Oscillator::frequencyToW(double dFrequency)
{
    return dFrequency * 2 * 3.14159;
}

double SynthesizerFromScratch::Oscillator::sampleSinWave(double dFrequency, double dTime)
{
    return 0.5 * sin(dFrequency);
}

double SynthesizerFromScratch::Oscillator::sampleSquareWave(double dFrequency, double dTime)
{
    return (sin(dFrequency) > 0) ? 0.1 : -0.1;
}

double SynthesizerFromScratch::Oscillator::sampleTriangleWave(double dFrequency, double dTime)
{
    return asin(sin(dFrequency) * 2.0 / 3.14159);
}

double SynthesizerFromScratch::Oscillator::sampleSawAnalogueWave(double dFrequency, double dTime)
{
    double dOutput = 0.0;

    for (double n = 1.0; n < 4; ++n)
        dOutput += (sin(n * dFrequency)) / n;

    return dOutput * (2.0 / 3.14159);
}

double SynthesizerFromScratch::Oscillator::sampleSawWave(double dFrequency, double dTime)
{
    return (2.0 / 3.14159) * (dFrequency * 3.14159 * fmod(dTime, 1.0 / dFrequency) - (3.14159 / 2.0));
}

double SynthesizerFromScratch::Oscillator::sampleNoise()
{
    return 2.0 * ((double)rand() / (double)RAND_MAX) - 1.0;
}