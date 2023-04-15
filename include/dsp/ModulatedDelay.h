/*
  ==============================================================================

    ModulatedDelay.h
    Created: 31 Aug 2020 12:54:44pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include <dsp/IAudioProcessor.h>
#include <dsp/Parameters.h>
#include <dsp/Delay.h>
#include <dsp/LFO.h>

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