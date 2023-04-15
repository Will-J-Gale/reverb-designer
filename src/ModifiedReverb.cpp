/*
  ==============================================================================

    ModifiedReverb.cpp
    Created: 5 Sep 2020 9:22:46pm
    Author:  Will

  ==============================================================================
*/

#include <dsp/ModifiedReverb.h>

double ModifiedReverb::process(double xn)
{
    double globalFeedback = lastBranchSample * parameters.rT;
    //globalFeedback += feedbackSample * 0.95;

    double tapDelayOutput = 0.0;

    for (size_t i = 0; i < NUM_TAPS; i++)
    {
        double delayTime = tapDelayTimes[i] * parameters.roomSize;
        double delayGain = tapDelayGains[i];

        tapDelayOutput += tapDelay.readDelayAtTimeInMs(delayTime) * delayGain;
    }
    tapDelay.write(xn);
    tapDelayOutput = tapLpf.process(tapDelayOutput);

    double input = tapDelayOutput + globalFeedback;

    for (size_t i = 0; i < NUM_BRANCHES; i++)
    {
        double apfOut = branchNestedAPFs[i].process(input);
        double lpfOut = branchLPFs[i].process(apfOut);

        input = lpfOut + tapDelayOutput;
    }

    double yn = (xn * 0.5) + (input * 0.5);
    feedbackSample = yn;
    return yn;    
}

bool ModifiedReverb::canProcessAudioFrame()
{
    return false;
}

void ModifiedReverb::reset(double sampleRate, double maxDelayTimeInSeconds)
{
    tapDelay.reset(sampleRate, maxDelayTimeInSeconds);
    tapLpf.reset(sampleRate);

    for (int i = 0; i < NUM_BRANCHES; i++)
    {
        branchNestedAPFs[i].reset(sampleRate, maxDelayTimeInSeconds);
        branchLPFs[i].reset(sampleRate);
    }
}

ModifiedReverbParameters ModifiedReverb::getParameters()
{
    return parameters;
}

void ModifiedReverb::setParameters(ModifiedReverbParameters newParameters)
{
    parameters = newParameters;

    //Tap delay
    auto lpfParams = tapLpf.getParameters();
    lpfParams.g = parameters.tapDamping;
    tapLpf.setParameters(lpfParams);

    //LPFs
    SimpleLPFParameters lpfParameters = branchLPFs[0].getParameters();
    lpfParameters.g = parameters.lpfG;

    for (int i = 0; i < NUM_BRANCHES; i++)
    {
        branchLPFs[i].setParameters(lpfParameters);
    }

    //Nested APF
    NestedDelayAPFParameters apfParams = branchNestedAPFs[0].getParameters();

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
    }
}
