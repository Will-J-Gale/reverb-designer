/*
  ==============================================================================

    Gain.cpp
    Created: 19 Sep 2020 1:26:50pm
    Author:  Will

  ==============================================================================
*/

#include <dsp/Gain.h>

bool Gain::reset(double sampleRate)
{
    return false;
}

double Gain::process(double xn)
{
    return xn * gain;
}

bool Gain::canProcessAudioFrame()
{
    return false;
}

void Gain::setGain(double gain)
{
    this->gain = gain;
}

double Gain::getGain()
{
    return gain;
}
