/*
  ==============================================================================

    Delay.h
    Created: 29 Aug 2020 2:42:35pm
    Author:  Will

  ==============================================================================
*/
#pragma once
#include "IAudioProcessor.h"
#include "CircularBuffer.h"
#include "Parameters.h"

class Delay : public IAudioProcessor
{
public:
    bool reset(double sampleRate);
    double process(double xn);
    bool canProcessAudioFrame();

    void setParameters(AudioDelayParameters parameters);
    AudioDelayParameters getParameters();

    void createDelayBuffers(double sampleRate, float bufferLengthInSeconds);

private:
    CircularBuffer buffer;
    AudioDelayParameters parameters;

    double sampleRate = 0.0;
    double delayInSamples = 0.0;
    double bufferLengthInSeconds = 5.0;
    size_t bufferLength = 0;
};