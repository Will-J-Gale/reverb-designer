#pragma once
#include <dsp/Parameters.h>
#include <dsp/IAudioProcessor.h>
#include <dsp/Delay.h>
#include <dsp/LFO.h>
#include <parameters/AudioParameters.h>
#include <parameters/Parameter.h>
#include <utils/Constants.h>

class DelayAPF : public IAudioProcessor
{
public:
    DelayAPF();
    virtual bool reset(double sampleRate) override;
    virtual double process(double xn) override;
    virtual bool canProcessAudioFrame() override;
    void onParametersChanged();

protected:
    Delay delay;
    AudioParametersPtr delayParameters = nullptr;
    LFO lfo;
    double lpfState = 0.0;
    double bufferLengthIsMs = 0.0;
    double sampleRate = 0.0;
};