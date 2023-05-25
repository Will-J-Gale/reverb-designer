#pragma once

#include <dsp/IAudioProcessor.h>
#include <dsp/BiquadFilter.h>
#include <dsp/Parameters.h>
#include <parameters/Parameter.h>
#include <utils/Constants.h>

class AudioFilter : public IAudioProcessor
{
public:
    AudioFilter();
    virtual void reset(double sampleRate);
    virtual double process(double xn);
    virtual bool canProcessAudioFrame();
    virtual void setSampleRate(double sampleRate);
    void onParametersChanged() override;

private:
    void calculateCoefficients();

private:
    BiquadFilter _biquad;
    double _sampleRate = DEFAULT_SAMPLERATE;
    double _coeffs[BiquadCoefficients::NUM_COEFFS] = { 0.0, 0.0, 0.0, 0.0, 0.0};
};