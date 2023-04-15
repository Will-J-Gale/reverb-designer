/*
  ==============================================================================

    IDelayProcessor.h
    Created: 2 Sep 2020 9:38:29pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include <dsp/IAudioProcessor.h>

class IDelayProcessor : public IAudioProcessor
{
public:
    virtual void reset(double sampleRate, double maxDelayTimeInSeconds) = 0;
private:
    virtual bool reset(double sampleRate) override { return false; }
};