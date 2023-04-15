/*
  ==============================================================================

    SimpleLPF.h
    Created: 31 Aug 2020 7:05:10pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include "Parameters.h"
#include "IAudioProcessor.h"

class SimpleLPF : IAudioProcessor
{
public:
    // Inherited via IAudioProcessor
    virtual bool reset(double sampleRate) override;
    virtual double process(double xn) override;
    virtual bool canProcessAudioFrame() override;

    SimpleLPFParameters getParameters();
    void setParameters(SimpleLPFParameters newParameters);

private:
    SimpleLPFParameters params;
    double state = 0.0;
};
