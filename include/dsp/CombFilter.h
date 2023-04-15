/*
  ==============================================================================

    CombFilter.h
    Created: 31 Aug 2020 7:16:27pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include <dsp/Parameters.h>
#include <dsp/IDelayProcessor.h>
#include <dsp/SimpleDelay.h>

class CombFilter : public IDelayProcessor
{
public:
    // Inherited via IAudioProcessor
    virtual void reset(double sampleRate, double delayTimeInSeconds) override;
    virtual double process(double xn) override;
    virtual bool canProcessAudioFrame() override;

    CombFilterParameters getParameters();
    void setParameters(CombFilterParameters newParams);

private:
    SimpleDelay delay;
    CombFilterParameters params;
    double combG = 0.0;
    double lpfG = 0.0;
    double lpfState = 0.0;
    double bufferLengthInMs = 0.0;
    double sampleRate = 0.0;
};