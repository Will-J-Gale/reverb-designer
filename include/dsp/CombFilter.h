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
    virtual bool reset(double sampleRate) override;
    virtual double process(double xn) override;
    virtual bool canProcessAudioFrame() override;
    void onParametersChanged() override;

private:
    Delay delay;
    AudioParametersPtr delayParameters = nullptr;
    double combG = 0.0;
    double lpfState = 0.0;
    double bufferLengthInMs = 0.0;
    double sampleRate = 0.0;
};