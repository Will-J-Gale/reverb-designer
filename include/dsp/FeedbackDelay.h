#pragma once
#include <dsp/IAudioProcessor.h>
#include <dsp/CircularBuffer.h>
#include <dsp/Parameters.h>
#include <parameters/AudioParameters.h>
#include <utils/Constants.h>

class FeedbackDelay : public IAudioProcessor
{
public:
    FeedbackDelay();
    bool reset(double sampleRate);
    double process(double xn);
    bool canProcessAudioFrame();
    void onParametersChanged() override;
    void createDelayBuffers(double sampleRate, float bufferLengthInSeconds);

private:
    CircularBuffer buffer;

    double sampleRate = 0.0;
    double delayInSamples = 0.0;
    double bufferLengthInSeconds = 5.0;
    size_t bufferLength = 0;
};