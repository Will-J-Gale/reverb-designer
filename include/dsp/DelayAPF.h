/*
  ==============================================================================

    DelayAPF.h
    Created: 2 Sep 2020 8:09:36pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include <dsp/Parameters.h>
#include <dsp/IDelayProcessor.h>
#include <dsp/Delay.h>
#include <dsp/LFO.h>
#include <parameters/AudioParameters.h>
#include <parameters/Parameter.h>
#include <utils/Constants.h>

class DelayAPF : public IDelayProcessor
{
public:
    DelayAPF();
    virtual void reset(double sampleRate, double maxDelayTimeInSeconds) override;
    virtual double process(double xn) override;
    virtual bool canProcessAudioFrame() override;
    virtual AudioParametersPtr getParameters() override;
    void onParametersChanged();

protected:
    AudioParametersPtr parameters = MAKE_PARAMETERS({
        std::make_shared<DoubleParameter>("DelayTimeMs", 0.0, APF_MIN_DELAY, APF_MAX_DELAY),
        std::make_shared<DoubleParameter>("APF G", 0.0, 0.0, 1.0),
        std::make_shared<BooleanParameter>("Enable LPF", false),
        std::make_shared<DoubleParameter>("LPF G", 0.0, 0.0, 1.0),
        std::make_shared<BooleanParameter>("Enable LFO", false),
        std::make_shared<DoubleParameter>("LFO Rate", 0.0, APF_LFO_RATE_MIN, APF_LFO_RATE_MAX),
        std::make_shared<DoubleParameter>("LFO Depth", 0.0, 0.0, 1.0),
        std::make_shared<DoubleParameter>("LFO Max Modulation ms", 0.0, APF_MIN_MOD_FREQ, APF_MAX_MOD_FREQ),
    });

    Delay delay;
    AudioParametersPtr delayParameters = nullptr;
    LFO lfo;
    double lpfState = 0.0;
    double bufferLengthIsMs = 0.0;
    double sampleRate = 0.0;
};