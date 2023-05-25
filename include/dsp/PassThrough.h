#pragma once
#include <dsp/Parameters.h>
#include <dsp/IAudioProcessor.h>
#include <parameters/AudioParameters.h>

class PassThrough : public IAudioProcessor
{
public:
    PassThrough();
    virtual void reset(double sampleRate) override;
    virtual double process(double xn) override;
    virtual bool canProcessAudioFrame() override;
    virtual AudioParametersPtr getParameters() override;
};