/*
  ==============================================================================

    DelayAPF.cpp
    Created: 2 Sep 2020 8:09:36pm
    Author:  Will

  ==============================================================================
*/

#include <dsp/DelayAPF.h>

void DelayAPF::reset(double sampleRate, double maxDelayTimeInSeconds)
{
    this->sampleRate = sampleRate;
    delay.reset(sampleRate, maxDelayTimeInSeconds);
    lfo.reset(sampleRate);
}

double DelayAPF::process(double xn)
{
    double g = parameters.apfG;
    double g2 = parameters.lpfG;
    double delayedSample = 0.0;

    if (parameters.enableLFO)
    {
        SignalGenData lfoOutput = lfo.renderAudioOutput();
        double lfoDepth = parameters.lfoDepth;

        double maxDelay = parameters.delayTimeInMs;
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
    double yn = (wn * -g) + delayedSample;

    /*
        This is different to the book's version
        The book does the filter on delayedSample
        I don't think it matters where the filter is
        because either the delay will already have filtering
        applied before it has been read, or after it has been read,
        resulting in xn always being added to a filtered version
    */

    if (parameters.enableLPF)
    {
        wn = (wn * (1 - g2)) + (lpfState * g2);
        lpfState = wn;
    }

    delay.write(wn);

    return yn;
}

bool DelayAPF::canProcessAudioFrame()
{
    return false;
}

DelayAPFParameters DelayAPF::getParameters()
{
    return parameters;
}

void DelayAPF::setParameters(DelayAPFParameters newParameters)
{
    parameters = newParameters;

    SimpleDelayParameters delayParams = delay.getParameters();
    delayParams.delayTimeInMs = parameters.delayTimeInMs;
    delay.setParameters(delayParams);

    OscillatorParameters lfoPrams = lfo.getParameters();
    lfoPrams.frequency = parameters.lfoRate;
    lfo.setParameters(lfoPrams);
}
