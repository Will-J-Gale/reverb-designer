/*
  ==============================================================================

    Delay.cpp
    Created: 29 Aug 2020 2:42:35pm
    Author:  Will

  ==============================================================================
*/

#include <dsp/Delay.h>

bool Delay::reset(double sampleRate)
{
    if (this->sampleRate == sampleRate)
    {
        buffer.flushBuffer();
        return true;
    }

    createDelayBuffers(sampleRate, bufferLengthInSeconds);
    return true;
}

double Delay::process(double xn)
{
    double delay = buffer.read(delayInSamples);
    double yn = (delay * parameters.mix) + (xn * (1.0 - parameters.mix));

    double feedback = delay * parameters.feedback;
    buffer.write(xn + feedback);

    return yn;
}

bool Delay::canProcessAudioFrame()
{
	return false;
}

void Delay::setParameters(AudioDelayParameters parameters)
{
    this->parameters = parameters;

    delayInSamples = sampleRate * parameters.delayInSeconds;
}

AudioDelayParameters Delay::getParameters()
{
    return parameters;
}

void Delay::createDelayBuffers(double sampleRate, float bufferLengthInSeconds)
{
    this->bufferLengthInSeconds = bufferLengthInSeconds;
    this->sampleRate = sampleRate;

    this->bufferLength = sampleRate * bufferLengthInSeconds;
    buffer.createCircularBuffer(bufferLength);
}
