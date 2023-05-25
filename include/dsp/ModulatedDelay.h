#pragma once
#include <dsp/IAudioProcessor.h>
#include <dsp/Parameters.h>
#include <dsp/FeedbackDelay.h>
#include <dsp/LFO.h>
#include <parameters/AudioParameters.h>
#include <utils/Constants.h>

class ModulatedDelay : public IAudioProcessor
{
public:
    ModulatedDelay();
    virtual void reset(double sampleRate) override;
    virtual double process(double xn) override;
    virtual bool canProcessAudioFrame() override;
    void onParametersChanged();

private:
    FeedbackDelay _delay;
    AudioParametersPtr _delayParameters = nullptr;
    LFO _lfo;
};