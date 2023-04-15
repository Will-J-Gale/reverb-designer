/*
  ==============================================================================

    SimpleLPF.cpp
    Created: 31 Aug 2020 7:05:10pm
    Author:  Will

  ==============================================================================
*/

#include <dsp/SimpleLPF.h>

bool SimpleLPF::reset(double sampleRate)
{
    state = 0.0;

    return true;
}

double SimpleLPF::process(double xn)
{
    double g = params.g;
    double yn = (xn * (1 - g)) + (state * g);

    state = yn;

    return yn;
}

bool SimpleLPF::canProcessAudioFrame()
{
    return false;
}

SimpleLPFParameters SimpleLPF::getParameters()
{
    return params;
}

void SimpleLPF::setParameters(SimpleLPFParameters newParameters)
{
    params = newParameters;
}
