/*
  ==============================================================================

    SimpleDelay.h
    Created: 31 Aug 2020 6:16:15pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include <dsp/Parameters.h>
#include <dsp/CircularBuffer.h>
#include <dsp/IDelayProcessor.h>

class SimpleDelay : public IDelayProcessor
{
public:
    // Inherited via IAudioProcessor
    virtual void reset(double sampleRate, double maxDelayTimeInSeconds) override;
    virtual double process(double xn) override;
    virtual bool canProcessAudioFrame() override;

    SimpleDelayParameters getParameters();
    void setParameters(SimpleDelayParameters newParameters);
    double readDelay();
    double readDelayAtTimeInMs(double delayInMs);
    double readDelayAtPercentage(double delayPercentage);
    void write(double xn);

private:
    SimpleDelayParameters params;
    CircularBuffer buffer;
    size_t bufferLength = 0;
    double delayTimeInSamples;
    double sampleRate;
    
};