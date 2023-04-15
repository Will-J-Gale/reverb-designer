/*
  ==============================================================================

    Gain.h
    Created: 19 Sep 2020 1:26:50pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include <dsp/IAudioProcessor.h>
#include <dsp/Parameters.h>

class Gain : public IAudioProcessor
{
public:
    // Inherited via IAudioProcessor
    virtual bool reset(double sampleRate) override;

    virtual double process(double xn) override;

    virtual bool canProcessAudioFrame() override;

    void setGain(double gain);
    double getGain();

private:

    double gain = 1.0;
};