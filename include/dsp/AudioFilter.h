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
    void onParametersChanged() override;

private:
    void calculateCoefficients();

private:
    BiquadFilter biquad;

    double sampleRate;
    double coeffs[numCoeffs] = { 0.0, 0.0, 0.0, 0.0, 0.0};
};