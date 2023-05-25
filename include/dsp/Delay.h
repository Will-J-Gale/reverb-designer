#pragma once
#include <dsp/Parameters.h>
#include <dsp/CircularBuffer.h>
#include <dsp/IAudioProcessor.h>
#include <utils/Constants.h>

class Delay : public IAudioProcessor
{
public:
    Delay();
    virtual void reset(double sampleRate) override;
    virtual double process(double xn) override;
    virtual bool canProcessAudioFrame() override;
    void onParametersChanged() override;

    double readDelay();
    double readDelayAtTimeInMs(double delayInMs);
    double readDelayAtPercentage(double delayPercentage);
    void write(double xn);

private:
    CircularBuffer _buffer;
    size_t _bufferLength = 0;
    double _sampleRate = DEFAULT_SAMPLERATE;
    double _delayTimeInSamples = 0.0;
    
};