/*
  ==============================================================================

    CombFilter.cpp
    Created: 31 Aug 2020 7:16:27pm
    Author:  Will

  ==============================================================================
*/

#include "CombFilter.h"
#include <cmath>

void CombFilter::reset(double sampleRate, double delayTimeInSeconds)
{
    this->sampleRate = sampleRate;
    delay.reset(sampleRate, delayTimeInSeconds);
    lpfState = 0.0;
}

double CombFilter::process(double xn)
{
    double yn = delay.readDelay();
    double input = 0.0;

    if (params.enableLpf)
    {
        double g2 = lpfG * (1.0 - combG);
        double filteredSignal = yn + (lpfState * g2);
        input = xn + (filteredSignal * combG);
        lpfState = filteredSignal;
    }
    else
    {
        input = xn + (yn * combG);
    }

    delay.write(input);

    return yn;
}

bool CombFilter::canProcessAudioFrame()
{
    return false;
}

CombFilterParameters CombFilter::getParameters()
{
    return params;
}

void CombFilter::setParameters(CombFilterParameters newParams)
{
    params = newParams;

    SimpleDelayParameters delayParams = delay.getParameters();
    delayParams.delayTimeInMs = params.delayTimeInMs;
    delayParams.interpolate = params.interpolate;
    delay.setParameters(delayParams);

    double exponent = -3.0 * params.delayTimeInMs * (1.0 / sampleRate);
    double rt60InSeconds = params.RT60TimeInMs / 1000.0;
    combG = std::pow(10, exponent / rt60InSeconds);

    lpfG = params.lpfG;
}