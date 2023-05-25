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
    virtual void reset(double sampleRate) override;
    virtual double process(double xn) override;
    virtual bool canProcessAudioFrame() override;
    void onParametersChanged();

protected:
    Delay _delay;
    AudioParametersPtr _delayParameters = nullptr;
    LFO _lfo;
    double _lpfState = 0.0;
    double _bufferLengthIsMs = 0.0;
    double _sampleRate = 0.0;
};