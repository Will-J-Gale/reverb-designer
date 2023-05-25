#pragma once
#include <dsp/Parameters.h>
#include <dsp/IAudioProcessor.h>
#include <dsp/Delay.h>
#include <parameters/AudioParameters.h>
#include <parameters/Parameter.h>

class CombFilter : public IAudioProcessor
{
public:
    CombFilter();
    virtual void reset(double sampleRate) override;
    virtual double process(double xn) override;
    virtual bool canProcessAudioFrame() override;
    void onParametersChanged() override;

private:
    Delay _delay;
    AudioParametersPtr _delayParameters = nullptr;
    double _combG = 0.0;
    double _lpfState = 0.0;
    double _bufferLengthInMs = 0.0;
    double _sampleRate = 0.0;
};