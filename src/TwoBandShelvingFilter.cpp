/*
  ==============================================================================

    TwoBandShelvingFilter.cpp
    Created: 3 Sep 2020 9:27:03pm
    Author:  Will

  ==============================================================================
*/

#include "TwoBandShelvingFilter.h"

bool TwoBandShelvingFilter::reset(double sampleRate)
{
    lowShelfFilter.reset(sampleRate);
    highShelfFilter.reset(sampleRate);

    auto lsfParams = lowShelfFilter.getParameters();
    auto hsfParams = highShelfFilter.getParameters();

    lsfParams.filterType = FilterType::LSF;
    hsfParams.filterType = FilterType::HSF;

    lowShelfFilter.setParameters(lsfParams);
    highShelfFilter.setParameters(hsfParams);

    return true;
}

double TwoBandShelvingFilter::process(double xn)
{
    double yn = 0.0;

    yn = lowShelfFilter.process(xn);
    yn = highShelfFilter.process(yn);

    return yn;
}

bool TwoBandShelvingFilter::canProcessAudioFrame()
{
    return false;
}

TwoBandShelvingFilterParameters TwoBandShelvingFilter::getParameters()
{
    return parameters;
}

void TwoBandShelvingFilter::setParameters(TwoBandShelvingFilterParameters newParameters)
{
    parameters = newParameters;

    auto lowShelfFilterParams = lowShelfFilter.getParameters();
    auto highShelfFilterParams = highShelfFilter.getParameters();

    lowShelfFilterParams.fc = parameters.lowSelfFreq;
    lowShelfFilterParams.gain_dB = parameters.lowShelfGainInDb;

    highShelfFilterParams.fc = parameters.highSelfFreq;
    highShelfFilterParams.gain_dB = parameters.highShelfGainInDb;

    lowShelfFilter.setParameters(lowShelfFilterParams);
    highShelfFilter.setParameters(highShelfFilterParams);
}
