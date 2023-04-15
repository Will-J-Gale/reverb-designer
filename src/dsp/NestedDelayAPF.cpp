/*
  ==============================================================================

    NestedDelayAPF.cpp
    Created: 3 Sep 2020 7:40:59pm
    Author:  Will

  ==============================================================================
*/

#include <dsp/NestedDelayAPF.h>

void NestedDelayAPF::reset(double sampleRate, double maxDelayTimeInSeconds)
{
    nestedApf.reset(sampleRate, maxDelayTimeInSeconds);
    DelayAPF::reset(sampleRate, maxDelayTimeInSeconds);
}

double NestedDelayAPF::process(double xn)
{
    double delayedSample = 0.0;
    double g = parameters.outerApfG;
    double lpfG = DelayAPF::parameters.lpfG;

    if (parameters.enableLFO)
    {
        SignalGenData lfoOutput = lfo.renderAudioOutput();
        double lfoDepth = parameters.lfoDepth;

        double maxDelay = parameters.outerDelayTimeInMs;
        double minDelay = maxDelay - parameters.lfoMaxModulationInMs;

        minDelay = fmax(minDelay, 0.0);

        double uniPolarLfo = Math::bipolarToUnipolar(lfoDepth * lfoOutput.normalOutput);
        double modDelayInMs = Math::uniPolarScale(uniPolarLfo, minDelay, maxDelay);

        delayedSample = delay.readDelayAtTimeInMs(modDelayInMs);
    }
    else
    {
        delayedSample = delay.readDelay();
    }

    double wn = xn + (delayedSample * g);

    if (DelayAPF::parameters.enableLPF)
    {
        wn = (wn * (1 - lpfG)) + (lpfState * lpfG);
        lpfState = wn;
    }

    double ynInner = nestedApf.process(wn);
    delay.write(ynInner);

    double yn = delayedSample + (-g * wn);
    return yn;
}

NestedDelayAPFParameters NestedDelayAPF::getParameters()
{
    return parameters;
}

void NestedDelayAPF::setParameters(NestedDelayAPFParameters newParameters)
{
    parameters = newParameters;

    DelayAPFParameters outerApfParams = DelayAPF::getParameters();
    DelayAPFParameters innerApfParams = nestedApf.getParameters();

    outerApfParams.apfG = parameters.outerApfG;
    innerApfParams.apfG = parameters.innerApfG;

    outerApfParams.delayTimeInMs = parameters.outerDelayTimeInMs;
    innerApfParams.delayTimeInMs = parameters.innerDelayTimeMs;

    outerApfParams.enableLPF = parameters.enableLPF;
    outerApfParams.lpfG = parameters.lpfG;

    outerApfParams.enableLFO = parameters.enableLFO;
    outerApfParams.lfoRate = parameters.lfoRateHz;
    outerApfParams.lfoDepth = parameters.lfoDepth;
    outerApfParams.lfoMaxModulationInMs = parameters.lfoMaxModulationInMs;

    DelayAPF::setParameters(outerApfParams);
    nestedApf.setParameters(innerApfParams);
}
