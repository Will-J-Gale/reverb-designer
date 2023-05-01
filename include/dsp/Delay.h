#pragma once
#include <dsp/Parameters.h>
#include <dsp/CircularBuffer.h>
#include <dsp/IAudioProcessor.h>

class Delay : public IAudioProcessor
{
public:
    Delay();
    virtual bool reset(double sampleRate) override;
    virtual double process(double xn) override;
    virtual bool canProcessAudioFrame() override;
    virtual AudioParametersPtr getParameters() override;
    void onParametersChanged() override;

    double readDelay();
    double readDelayAtTimeInMs(double delayInMs);
    double readDelayAtPercentage(double delayPercentage);
    void write(double xn);

private:
    AudioParametersPtr parameters = MAKE_PARAMETERS({
        std::make_shared<DoubleParameter>("DelayTimeMs", 0.0, 0.0, 5000.0)
    });

    CircularBuffer buffer;
    size_t bufferLength = 0;
    double sampleRate;
    double delayTimeInSamples;
    
};