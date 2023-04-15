/*
  ==============================================================================

    LRFilterBank.h
    Created: 6 Sep 2020 4:41:37pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include "Parameters.h"
#include "AudioFilter.h"

class LRFilterBank
{
public:
    LRFilterBank();
    bool reset(double sampleRate);

    LRFilterBankParameters getParameters();
    void setParameters(LRFilterBankParameters newParameters);
    LRFilterBankOutput processFilterBank(double xn);

private:
    LRFilterBankParameters parameters;
    AudioFilter lpf;
    AudioFilter hpf;
};