#pragma once
#include <dsp/Parameters.h>
#include <dsp/IAudioProcessor.h>

class SimpleLPF : IAudioProcessor
{
public:
    SimpleLPF();
    virtual void reset(double sampleRate) override;
    virtual double process(double xn) override;
    virtual bool canProcessAudioFrame() override;
    AudioParametersPtr getParameters() override;

    SimpleLPFParameters getLPFParameters();
    void setParameters(SimpleLPFParameters newParameters);

private:
    SimpleLPFParameters _params;
    double _state = 0.0;
};
