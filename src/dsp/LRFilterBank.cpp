/*
  ==============================================================================

    LRFilterBank.cpp
    Created: 6 Sep 2020 4:41:37pm
    Author:  Will

  ==============================================================================
*/

#include <dsp/LRFilterBank.h>

LRFilterBank::LRFilterBank()
{
    auto lpfParams = lpf.getParameters();
    auto hpfParams = hpf.getParameters();

    lpfParams.filterType = FilterType::LWRLPF2;
    hpfParams.filterType = FilterType::LWRHPF2;

    lpf.setParameters(lpfParams);
    hpf.setParameters(hpfParams);
}

bool LRFilterBank::reset(double sampleRate)
{
    lpf.reset(sampleRate);
    hpf.reset(sampleRate);

    return true;
}

LRFilterBankParameters LRFilterBank::getParameters()
{
    return LRFilterBankParameters();
}

void LRFilterBank::setParameters(LRFilterBankParameters newParameters)
{
    parameters = newParameters;

    auto lpfParams = lpf.getParameters();
    auto hpfParams = hpf.getParameters();

    lpfParams.fc = parameters.splitFrequency;
    hpfParams.fc = parameters.splitFrequency;

    lpf.setParameters(lpfParams);
    hpf.setParameters(hpfParams);
}

LRFilterBankOutput LRFilterBank::processFilterBank(double xn)
{
    LRFilterBankOutput output;

    output.LFOut = lpf.process(xn);
    output.HFOut = -hpf.process(xn);

    return output;
}
