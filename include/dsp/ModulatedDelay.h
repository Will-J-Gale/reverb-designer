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
    virtual bool reset(double sampleRate) override;
    virtual double process(double xn) override;
    virtual bool canProcessAudioFrame() override;
    void onParametersChanged();

private:
    FeedbackDelay delay;
    AudioParametersPtr delayParameters = nullptr;
    LFO lfo;
};