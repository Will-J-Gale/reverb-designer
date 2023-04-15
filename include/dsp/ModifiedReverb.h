/*
  ==============================================================================

    ModifiedReverb.h
    Created: 5 Sep 2020 9:22:46pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include <dsp/IDelayProcessor.h>
#include <dsp/SimpleDelay.h>
#include <dsp/SimpleLPF.h>
#include <dsp/NestedDelayAPF.h>

class ModifiedReverb : IDelayProcessor
{
public:
    // Inherited via IDelayProcessor
    virtual double process(double xn) override;
    virtual bool canProcessAudioFrame() override;
    virtual void reset(double sampleRate, double maxDelayTimeInSeconds) override;
    ModifiedReverbParameters getParameters();
    void setParameters(ModifiedReverbParameters newParameters);

private:
    SimpleDelay tapDelay;
    SimpleLPF tapLpf;
    ModifiedReverbParameters parameters;

    NestedDelayAPF branchNestedAPFs[NUM_BRANCHES];
    SimpleLPF branchLPFs[NUM_BRANCHES];

    double feedbackSample = 0.0;

    double lastBranchSample = 0.0;
    double apfDelayWeight[NUM_BRANCHES * 2] = { 0.317, 0.873, 0.477, 0.291, 0.993, 0.757, 0.179, 0.575 };
    double lfoRates[NUM_BRANCHES] = { 0.15, 0.33, 0.57, 0.73 };

    double NUM_TAPS = 18;
    double tapDelayTimes[18] = { 43, 215, 225, 268,  270, 298, 458, 485,  572, 587, 595, 612, 707, 708, 726, 741, 753, 797 };
    double tapDelayGains[18] = { 0.841, 0.504, 0.491, 0.379, 0.38, 0.346, 0.289, 0.272, 0.192, 0.192, 0.217, 0.181, 0.180, 0.181, 0.176, 0.142, 0.167, 0.134 };
};
