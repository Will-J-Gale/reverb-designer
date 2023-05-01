#pragma once
#include <dsp/Parameters.h>

class IAudioSignalGenerator
{
public:
    virtual void reset(double sampleRate) = 0;
    virtual void setGeneratorFrequency(double frequency) = 0;
    virtual void setGeneratorWaveform(GeneratorWaveform waveform) = 0;
    virtual SignalGenData renderAudioOutput() = 0;
};