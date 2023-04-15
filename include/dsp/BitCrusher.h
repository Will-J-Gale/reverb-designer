/*
  ==============================================================================

    BitCrusher.h
    Created: 7 Sep 2020 1:42:51pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include <dsp/Parameters.h>
#include <dsp/IAudioProcessor.h>

//Bit crushing sounds crap
//Sample rate crushing sounds better

class BitCrusher : IAudioProcessor
{
public:
    // Inherited via IAudioProcessor
    virtual bool reset(double sampleRate) override;
    virtual double process(double xn) override;
    virtual bool canProcessAudioFrame() override;

    BitChrusherParameters getParameters();
    void setParameters(BitChrusherParameters newParameters);

private:
    BitChrusherParameters parameters;
    double qL = 0.0;
    double phasor = 0.0;
    double heldSample = 0.0;
};