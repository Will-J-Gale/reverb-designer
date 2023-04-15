/*
  ==============================================================================

    ReverbTank.h
    Created: 5 Sep 2020 3:14:46pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include "IDelayProcessor.h"
#include "Parameters.h"
#include "SimpleDelay.h"
#include "NestedDelayAPF.h"
#include "SimpleLPF.h"
#include "TwoBandShelvingFilter.h"

class ReverbTank : IDelayProcessor
{
public:
    virtual void reset(double sampleRate, double maxDelayTimeInSeconds) override;
    virtual double process(double xn) override;
    virtual bool processAudioFrame(const float* inputFrame,
        float* outputFrame,
        uint32_t inputChannels,
        uint32_t outputChannels) override;

    virtual bool canProcessAudioFrame() override;

    ReverbTankParameters getParameters();
    void setParameters(ReverbTankParameters newParameters);
private:
    ReverbTankParameters parameters;
    SimpleDelay preDelay;
    SimpleDelay branchDelays[NUM_BRANCHES];
    NestedDelayAPF branchNestedAPFs[NUM_BRANCHES];
    SimpleLPF branchLPFs[NUM_BRANCHES];
    TwoBandShelvingFilter shelvingFilters[NUM_CHANNELS];
    double sampleRate = 0.0;

    //Might be better moving this into parameters
    //Or have a way of setting these in a more dynamic way
    double apfDelayWeight[NUM_BRANCHES * 2] = { 0.317, 0.873, 0.477, 0.291, 0.993, 0.757, 0.179, 0.575 };///< weighting values to make various and low-correlated APF delay values easily
    double fixedDelayWeight[NUM_BRANCHES] = { 1.0, 0.873, 0.707, 0.667 };
    double lfoRates[NUM_BRANCHES] = { 0.15, 0.33, 0.57, 0.73 };

    double leftDelays[NUM_BRANCHES*2] = { 0.23, 0.41, 0.59, 0.73,  0.31, 0.47, 0.67, 0.83 };
    double rightDelays[NUM_BRANCHES*2] = { 0.29, 0.43, 0.61, 0.79, 0.37, 0.53, 0.71, 0.89 };
};