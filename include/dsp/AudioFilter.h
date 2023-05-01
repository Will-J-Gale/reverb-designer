/*
  ==============================================================================

    AudioFilter.h
    Created: 22 Aug 2020 2:02:10pm
    Author:  Will

  ==============================================================================
*/
#pragma once

#include <dsp/IAudioProcessor.h>
#include <dsp/BiquadFilter.h>
#include <dsp/Parameters.h>
#include <parameters/Parameter.h>

class AudioFilter : public IAudioProcessor
{
public:
    AudioFilter();
    virtual bool reset(double sampleRate);
    virtual double process(double xn);
    virtual bool canProcessAudioFrame();
    virtual void setSampleRate(double sampleRate);
    virtual AudioParametersPtr getParameters() override;
    void onParametersChanged() override;

private:
    void calculateCoefficients();

private:
    AudioParametersPtr parameters = MAKE_PARAMETERS({
        std::make_shared<DoubleParameter>("Freq", DEFAULT_FC, 20.0, 20000.0),
        std::make_shared<DoubleParameter>("Q", DEFAULT_Q, 0.01, 1.0),
        std::make_shared<DoubleParameter>("Gain", DEFAULT_GAIN, -10.0, 10.0),
        std::make_shared<OptionParameter>(
            "FilterType", 
            Array<OptionItem> {
                OptionItem("LPF", (int)FilterType::LPF1),
                OptionItem("HPF", (int)FilterType::HPF1),
                OptionItem("HSF", (int)FilterType::HSF),
                OptionItem("LSF", (int)FilterType::LSF),
            }, 
            0
        )
    });
    BiquadFilter biquad;

    double sampleRate;
    double coeffs[numCoeffs] = { 0.0, 0.0, 0.0, 0.0, 0.0};
};
