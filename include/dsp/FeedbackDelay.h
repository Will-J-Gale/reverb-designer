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
    void reset(double sampleRate);
    double process(double xn);
    bool canProcessAudioFrame();
    void onParametersChanged() override;
    void createDelayBuffers(double sampleRate, float bufferLengthInSeconds);

private:
    CircularBuffer _buffer;
    double _sampleRate = DEFAULT_SAMPLERATE;
    double _delayInSamples = 0.0;
    double _bufferLengthInSeconds = 5.0;
    size_t _bufferLength = 0;
};