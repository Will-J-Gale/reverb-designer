/*
  ==============================================================================

    SimpleDelay.cpp
    Created: 31 Aug 2020 6:16:15pm
    Author:  Will

  ==============================================================================
*/

#include <dsp/SimpleDelay.h>

void SimpleDelay::reset(double sampleRate, double maxDelayTimeInSeconds)
{
    this->sampleRate = sampleRate;

    buffer.flushBuffer();
    bufferLength = (sampleRate * maxDelayTimeInSeconds) + 1;

    buffer.createCircularBuffer(bufferLength);
}

double SimpleDelay::process(double xn)
{
    if (delayTimeInSamples == 0)
    {
        return xn;
    }

    double yn = buffer.read(delayTimeInSamples);

    buffer.write(xn);

    return yn;
}

bool SimpleDelay::canProcessAudioFrame()
{
    return false;
}

SimpleDelayParameters SimpleDelay::getParameters()
{
    return params;
}

void SimpleDelay::setParameters(SimpleDelayParameters newParameters)
{
    params = newParameters;
    delayTimeInSamples = sampleRate * (params.delayTimeInMs / 1000.0);

    buffer.setInterpolate(params.interpolate);
}

double SimpleDelay::readDelay()
{
    return buffer.read(delayTimeInSamples);
}

double SimpleDelay::readDelayAtTimeInMs(double delayInMs)
{
    double delayInSamples = sampleRate * (delayInMs / 1000.0);

    return buffer.read(delayInSamples);
}

double SimpleDelay::readDelayAtPercentage(double delayPercentage)
{
    double delayInSamples = delayTimeInSamples * delayPercentage;

    return buffer.read(delayInSamples);
}

void SimpleDelay::write(double xn)
{
    buffer.write(xn);
}
