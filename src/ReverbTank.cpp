/*
  ==============================================================================

    ReverbTank.cpp
    Created: 5 Sep 2020 3:14:46pm
    Author:  Will

  ==============================================================================
*/

#include "ReverbTank.h"

void ReverbTank::reset(double sampleRate, double maxDelayTimeInSeconds)
{
    this->sampleRate = sampleRate;

    preDelay.reset(sampleRate, maxDelayTimeInSeconds);

    for (int i = 0; i < NUM_BRANCHES; i++)
    {
        branchDelays[i].reset(sampleRate, maxDelayTimeInSeconds);
        branchNestedAPFs[i].reset(sampleRate, maxDelayTimeInSeconds);
        branchLPFs[i].reset(sampleRate);
    }

    for (int i = 0; i < NUM_CHANNELS; i++)
    {
        shelvingFilters[i].reset(sampleRate);
    }
}

double ReverbTank::process(double xn)
{
    return 0.0;
}

bool ReverbTank::processAudioFrame(const float* inputFrame, float* outputFrame, uint32_t inputChannels, uint32_t outputChannels)
{
    double lastDelay = branchDelays[NUM_BRANCHES - 1].readDelay();
    double globalFeedback = lastDelay * parameters.rT;

    double xnL = inputFrame[0];
    double xnR = inputChannels > 1 ? inputFrame[1]: 0.0;

    double inputScale = double(1.0 / inputChannels);
    double monoXn = (inputScale * xnL) + (inputScale * xnR);

    double preDelayOut = preDelay.process(monoXn);

    double input = preDelayOut + globalFeedback;

    for (size_t i = 0; i < NUM_BRANCHES; i++)
    {
        double apfOut = branchNestedAPFs[i].process(input);
        double lpfOut = branchLPFs[i].process(apfOut);
        double delayOut = branchDelays[i].process(lpfOut) * parameters.rT;

        input = delayOut + preDelayOut;
    }

    double weight = 0.707;
    double outL = 0.0;
    double outR = 0.0;

    for (size_t i = 0; i < NUM_BRANCHES; i++)
    {
        outL += branchDelays[i].readDelayAtPercentage(leftDelays[i]) * weight;
        outR += branchDelays[i].readDelayAtPercentage(rightDelays[i]) * weight;
    }

    if (parameters.reverbDensity == ReverbDensity::Thick)
    {
        for (size_t i = 0; i < NUM_BRANCHES; i++)
        {
            outL += branchDelays[i].readDelayAtPercentage(leftDelays[i + 4]) * weight;
            outR += branchDelays[i].readDelayAtPercentage(rightDelays[i + 4]) * weight;
        }
    }

    double tankOutL = shelvingFilters[0].process(outL);
    double tankOutR = shelvingFilters[1].process(outL);

    double dry = pow(10.0, parameters.dryLevelDb / 20.0);
    double wet = pow(10.0, parameters.wetLevelDb / 20.0);

    if (outputChannels == 1)
    {
        double tankOutput = (tankOutL * 0.5 + tankOutR * 0.5);
        outputFrame[0] = (xnL * dry) + (tankOutput * dry);
    }
    else
    {
        outputFrame[0] = (xnL * dry) + (tankOutL * wet);
        outputFrame[1] = (xnR * dry) + (tankOutR * wet);
    }

    return true;
}

bool ReverbTank::canProcessAudioFrame()
{
    return true;
}

ReverbTankParameters ReverbTank::getParameters()
{
    return parameters;
}

void ReverbTank::setParameters(ReverbTankParameters newParameters)
{
    parameters = newParameters;

    //Shelving filters
    TwoBandShelvingFilterParameters filterParams = shelvingFilters[0].getParameters();
    filterParams.highSelfFreq = parameters.highShelfFreq;
    filterParams.lowSelfFreq = parameters.lowShelfFreq;
    filterParams.highShelfGainInDb = parameters.highShelfGain;
    filterParams.lowShelfGainInDb = parameters.lowShelfGain;

    shelvingFilters[0].setParameters(filterParams);
    shelvingFilters[1].setParameters(filterParams);

    //LPFs
    SimpleLPFParameters lpfParameters = branchLPFs[0].getParameters();
    lpfParameters.g = parameters.lpfG;

    for (int i = 0; i < NUM_BRANCHES; i++)
    {
        branchLPFs[i].setParameters(lpfParameters);
    }

    //Pre delay
    SimpleDelayParameters preDelayParams = preDelay.getParameters();
    preDelayParams.delayTimeInMs = parameters.preDelayTimeInMs;
    preDelay.setParameters(preDelayParams);

    NestedDelayAPFParameters apfParams = branchNestedAPFs[0].getParameters();
    SimpleDelayParameters delayParams = branchDelays[0].getParameters();

    double globalAPFMaxDelay = parameters.apfDelayMaxInMs * parameters.apfDelayWeight;
    double globalFixedMaxDelay = parameters.fixedDelayMaxInMs * parameters.fixedDelayWeight;

    apfParams.enableLFO = true;
    apfParams.lfoMaxModulationInMs = 0.3;
    apfParams.lfoDepth = 1.0;

    size_t m = 0;
    for (int i = 0; i < NUM_BRANCHES; i++)
    {
        apfParams.outerDelayTimeInMs = globalAPFMaxDelay * apfDelayWeight[m++];
        apfParams.innerDelayTimeMs = globalAPFMaxDelay * apfDelayWeight[m++];
        apfParams.outerApfG = -0.5;
        apfParams.innerApfG = 0.5;
        apfParams.lfoRateHz = lfoRates[i];

        branchNestedAPFs[i].setParameters(apfParams);

        delayParams.delayTimeInMs = globalFixedMaxDelay * fixedDelayWeight[i];
        branchDelays[i].setParameters(delayParams);
    }
}