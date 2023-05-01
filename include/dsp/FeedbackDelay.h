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
    AudioParametersPtr getParameters() override;
    void onParametersChanged() override;

    void createDelayBuffers(double sampleRate, float bufferLengthInSeconds);

private:
    AudioParametersPtr parameters = MAKE_PARAMETERS({
        std::make_shared<DoubleParameter>("Mix", DEFAULT_MIX, 0.0, 1.0),
        std::make_shared<DoubleParameter>("DelayInSeconds", DEFAULT_DELAY_TIME, 0.0, 5.0),
        std::make_shared<DoubleParameter>("Feedback", DEFAULT_FEEDBACK, 0.0, 1.0),
    });
    CircularBuffer buffer;

    double sampleRate = 0.0;
    double delayInSamples = 0.0;
    double bufferLengthInSeconds = 5.0;
    size_t bufferLength = 0;
};