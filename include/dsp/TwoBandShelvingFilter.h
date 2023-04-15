/*
  ==============================================================================

    TwoBandShelvingFilter.h
    Created: 3 Sep 2020 9:27:03pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include <dsp/Parameters.h>
#include <dsp/IAudioProcessor.h>
#include <dsp/AudioFilter.h>

class TwoBandShelvingFilter : IAudioProcessor
{
public:
    virtual bool reset(double sampleRate) override;
    virtual double process(double xn) override;
    virtual bool canProcessAudioFrame() override;

    TwoBandShelvingFilterParameters getParameters();
    void setParameters(TwoBandShelvingFilterParameters newParameters);

private:
    TwoBandShelvingFilterParameters parameters;
    AudioFilter lowShelfFilter;
    AudioFilter highShelfFilter;
};