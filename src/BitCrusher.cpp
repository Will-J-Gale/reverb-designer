/*
  ==============================================================================

    BitCrusher.cpp
    Created: 7 Sep 2020 1:42:51pm
    Author:  Will

  ==============================================================================
*/

#include <dsp/BitCrusher.h>
#include <cmath>

bool BitCrusher::reset(double sampleRate)
{
    return true;
}

double BitCrusher::process(double xn)
{
    phasor += 1.0;

    if (phasor >= parameters.downSample)
    {
        heldSample = xn;
        phasor = 0.0;
    }

    return heldSample;

    //double bitCrush = qL * (int(xn / qL));
}

bool BitCrusher::canProcessAudioFrame()
{
    return false;
}

BitChrusherParameters BitCrusher::getParameters()
{
    return parameters;
}

void BitCrusher::setParameters(BitChrusherParameters newParameters)
{
    parameters = newParameters;

    qL = 2.0 / (pow(2.0, parameters.bitDepth) - 1.0);
}
