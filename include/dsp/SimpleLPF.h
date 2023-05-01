/*
  ==============================================================================

    SimpleLPF.h
    Created: 31 Aug 2020 7:05:10pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include <dsp/Parameters.h>
#include <dsp/IAudioProcessor.h>

class SimpleLPF : IAudioProcessor
{
public:
    SimpleLPF();
    virtual bool reset(double sampleRate) override;
    virtual double process(double xn) override;
    virtual bool canProcessAudioFrame() override;
    AudioParametersPtr getParameters() override;

    SimpleLPFParameters getLPFParameters();
    void setParameters(SimpleLPFParameters newParameters);

private:
    SimpleLPFParameters params;
    double state = 0.0;
};
