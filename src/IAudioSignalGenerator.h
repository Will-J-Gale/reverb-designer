/*
  ==============================================================================

    IAudioSignalGenerator.h
    Created: 29 Aug 2020 4:03:35pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include "Parameters.h"

class IAudioSignalGenerator
{
public:
    virtual void reset(double sampleRate) = 0;
    virtual void setGeneratorFrequency(double frequency) = 0;
    virtual void setGeneratorWaveform(GeneratorWaveform waveform) = 0;
    virtual SignalGenData renderAudioOutput() = 0;
};