/*
  ==============================================================================

    TriodeClassA.h
    Created: 7 Sep 2020 2:32:55pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include "IAudioProcessor.h"
#include "Parameters.h"
#include "AudioFilter.h"

class TriodeClassA : IAudioProcessor
{
public:
    TriodeClassA();

    // Inherited via IAudioProcessor
    virtual bool reset(double sampleRate) override;
    virtual double process(double xn) override;
    virtual bool canProcessAudioFrame() override;

    TriodeClassAParameters getParameters();
    void setParameters(TriodeClassAParameters newParameters);

private:
    TriodeClassAParameters parameters;
    AudioFilter hpf;
    AudioFilter lsf;

};