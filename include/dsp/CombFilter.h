/*
  ==============================================================================

    CombFilter.h
    Created: 31 Aug 2020 7:16:27pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include <dsp/Parameters.h>
#include <dsp/IDelayProcessor.h>
#include <dsp/Delay.h>
#include <parameters/AudioParameters.h>
#include <parameters/Parameter.h>

class CombFilter : public IDelayProcessor
{
public:
    CombFilter();
    // Inherited via IAudioProcessor
    virtual void reset(double sampleRate, double delayTimeInSeconds) override;
    virtual double process(double xn) override;
    virtual bool canProcessAudioFrame() override;
    virtual AudioParametersPtr getParameters() override;
    void onParametersChanged() override;

private:
    AudioParametersPtr parameters = MAKE_PARAMETERS({
        std::make_shared<DoubleParameter>("DelayTimeMs", 0.0, 0.0, 1000.0),
        std::make_shared<DoubleParameter>("RT60 Ms", 0.0, 0.0, 1000.0),
        std::make_shared<BooleanParameter>("Enable LPF", false),
        std::make_shared<DoubleParameter>("LPF G", 0.0, 0.0, 1.0)
    });
    Delay delay;
    AudioParametersPtr delayParameters = nullptr;
    double combG = 0.0;
    double lpfState = 0.0;
    double bufferLengthInMs = 0.0;
    double sampleRate = 0.0;
};