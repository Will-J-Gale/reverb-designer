/*
  ==============================================================================

    LFO.cpp
    Created: 29 Aug 2020 4:02:20pm
    Author:  Will

  ==============================================================================
*/
#include <cmath>
#include <dsp/LFO.h>

void LFO::reset(double sampleRate)
{
    this->sampleRate = sampleRate;
    phaseInc = lfoParameters.frequency / sampleRate;

    modCounter = 0.0;
    modCounterQP = 0.25;
}

void LFO::setGeneratorFrequency(double frequency)
{
}

void LFO::setGeneratorWaveform(GeneratorWaveform waveform)
{
}

SignalGenData LFO::renderAudioOutput()
{
    checkAndWrapModulo(modCounter, phaseInc);

    modCounterQP = modCounter;
    advanceAndcheckAndWrapModulo(modCounterQP, 0.25);

    SignalGenData output;

    if (lfoParameters.waveForm == GeneratorWaveform::kSin)
    {
        double angle = (modCounter * 2.0 * PI) - PI;

        output.normalOutput = parabolicSine(-angle);

        angle = (modCounterQP * 2.0 * PI) - PI;

        output.quadPhaseOutput_pos = parabolicSine(-angle);
    }
    else if (lfoParameters.waveForm == GeneratorWaveform::kTriangle)
    {
        double bipolar = unipolarToBipolar(modCounter);
        output.normalOutput = 2.0 * fabs(bipolar) - 1;

        bipolar = unipolarToBipolar(modCounterQP);
        output.quadPhaseOutput_pos = 2.0 * fabs(bipolar) - 1;
    }
    else if (lfoParameters.waveForm == GeneratorWaveform::kSaw)
    {
        output.normalOutput = unipolarToBipolar(modCounter);
        output.quadPhaseOutput_pos = unipolarToBipolar(modCounterQP);
    }

    output.invertedOutput = -output.normalOutput;
    output.quadPhaseOutput_neg = -output.quadPhaseOutput_pos;

    advanceModulo(modCounter, phaseInc);

    return output;
}

OscillatorParameters LFO::getParameters()
{
    return lfoParameters;
}

bool LFO::setParameters(OscillatorParameters newParameters)
{
    lfoParameters = newParameters;
    phaseInc = lfoParameters.frequency / sampleRate;
    return true;
}

void LFO::setSampleRate(double sampleRate)
{
    this->sampleRate = sampleRate;
}

bool LFO::checkAndWrapModulo(double& moduloCounter, double phaseInc)
{
    if (phaseInc > 0 && moduloCounter >= 1.0)
    {
        moduloCounter -= 1.0;
        return true;
    }

    // --- for negative frequencies
    if (phaseInc < 0 && moduloCounter <= 0.0)
    {
        moduloCounter += 1.0;
        return true;
    }

    return false;
}

bool LFO::advanceAndcheckAndWrapModulo(double& moduloCounter, double phaseInc)
{
    // --- advance counter
    moduloCounter += phaseInc;

    // --- for positive frequencies
    if (phaseInc > 0 && moduloCounter >= 1.0)
    {
        moduloCounter -= 1.0;
        return true;
    }

    // --- for negative frequencies
    if (phaseInc < 0 && moduloCounter <= 0.0)
    {
        moduloCounter += 1.0;
        return true;
    }

    return false;
}

void LFO::advanceModulo(double& moduloCounter, double phaseInc)
{
    moduloCounter += phaseInc;
}

double LFO::parabolicSine(double angle)
{
    double y = B * angle + C * angle * fabs(angle);
    y = P * (y * fabs(y) - y) + y;
    return y;
}
