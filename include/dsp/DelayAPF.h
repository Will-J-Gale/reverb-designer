/*
  ==============================================================================

    DelayAPF.h
    Created: 2 Sep 2020 8:09:36pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include <dsp/Parameters.h>
#include <dsp/IDelayProcessor.h>
#include <dsp/SimpleDelay.h>
#include <dsp/LFO.h>

class DelayAPF : public IDelayProcessor
{
public:
    // Inherited via IAudioProcessor
    
    virtual void reset(double sampleRate, double maxDelayTimeInSeconds) override;
    virtual double process(double xn) override;
    virtual bool canProcessAudioFrame() override;

    DelayAPFParameters getParameters();
    void setParameters(DelayAPFParameters newParameters);

protected:
    DelayAPFParameters parameters;
    SimpleDelay delay;
    LFO lfo;
    double lpfState = 0.0;
    double bufferLengthIsMs = 0.0;
    double sampleRate = 0.0;
};