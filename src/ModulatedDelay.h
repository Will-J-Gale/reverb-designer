/*
  ==============================================================================

    ModulatedDelay.h
    Created: 31 Aug 2020 12:54:44pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include "IAudioProcessor.h"
#include "Parameters.h"
#include "Delay.h"
#include "LFO.h"

class ModulatedDelay : public IAudioProcessor
{
public:
    virtual bool reset(double sampleRate) override;
    virtual double process(double xn) override;
    virtual bool canProcessAudioFrame() override;

    void setParamters(ModulatedDelayParameters newParameters);
    ModulatedDelayParameters getParameters();

private:
    ModulatedDelayParameters parameters;
    Delay delay;
    LFO lfo;
};