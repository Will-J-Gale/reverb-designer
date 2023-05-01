#pragma once
#include <dsp/IAudioProcessor.h>
#include <dsp/Parameters.h>
#include <parameters/AudioParameters.h>

class Gain : public IAudioProcessor
{
public:
    Gain();
    virtual bool reset(double sampleRate) override;
    virtual double process(double xn) override;
    virtual bool canProcessAudioFrame() override;
    virtual AudioParametersPtr getParameters() override;

private:
    AudioParametersPtr parameters = MAKE_PARAMETERS({
        std::make_shared<DoubleParameter>("Gain", 1.0, 0.0, 1.0),
    });
};