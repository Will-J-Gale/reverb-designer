/*
  ==============================================================================

    AudioDetector.cpp
    Created: 30 Aug 2020 12:43:05pm
    Author:  Will

  ==============================================================================
*/

#include "AudioDetector.h"
#include <cmath>

bool AudioDetector::reset(double sampleRate)
{
    this->sampleRate = sampleRate;
    lastEnvelope = 0.0;

    return true;
}

double AudioDetector::process(double xn)
{
    double input = fabs(xn);

    if (parameters.detectMode == AudioDetectorMode::kMS ||
        parameters.detectMode == AudioDetectorMode::kRMS)
    {
        input *= input;
    }

    double currentEnvelope = 0.0;

    if (input > lastEnvelope)
    {
        currentEnvelope = attackCoeff * (lastEnvelope - input) + input;
    }
    else
    {
        currentEnvelope = releaseCoeff * (lastEnvelope - input) + input;
    }

    if (parameters.clampToUnityMax)
    {
        currentEnvelope = fmin(currentEnvelope, 1.0);
    }

    currentEnvelope = fmax(currentEnvelope, 0.0);

    lastEnvelope = currentEnvelope;

    if (parameters.detectMode == AudioDetectorMode::kRMS)
    {
        currentEnvelope = std::pow(currentEnvelope, 0.5);
    }

    if (!parameters.detectDb)
    {
        return currentEnvelope;
    }

    if (currentEnvelope <= 0) { return -96; }

    return 20 * log10(currentEnvelope);
}

bool AudioDetector::canProcessAudioFrame()
{
	return false;
}

void AudioDetector::setParameters(AudioDetectorParameters newParameters)
{
    parameters = newParameters;
    setAttackTime(parameters.attackTimeInMs);
    setReleaseTime(parameters.releaseTimeInMs);
}

AudioDetectorParameters AudioDetector::getParameters()
{
    return parameters;
}

void AudioDetector::setSampleRate(double newSampleRate)
{
    sampleRate = newSampleRate;
}

void AudioDetector::setAttackTime(double newAttackTimeInMs)
{
    attackCoeff = exp(TLD_AUDIO_ENVELOPE_ANALOG_TC / (sampleRate * parameters.attackTimeInMs * 0.001));
}

void AudioDetector::setReleaseTime(double newReleaseTimeInMs)
{
    releaseCoeff = exp(TLD_AUDIO_ENVELOPE_ANALOG_TC / (sampleRate * parameters.releaseTimeInMs * 0.001));
}
