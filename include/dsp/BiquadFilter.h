#pragma once

#include <dsp/Parameters.h>
#include <parameters/AudioParameters.h>
#include <dsp/IAudioProcessor.h>
#include <utils/Constants.h>

class BiquadFilter : IAudioProcessor
{
public:
    BiquadFilter();
    ~BiquadFilter();

    virtual void reset(double sampleRate) override;
    double process(double xn) override;
    virtual bool canProcessAudioFrame() { return false; };
    virtual AudioParametersPtr getParameters() override;

    void setCoefficients(double* coeffs);
    void setAlgorithm(BiquadAlgorithm algorithm);

private:
    //Algorithms
    double processDirectForm(double xn);
    double processDirectFormTransposed(double xn);
    double processCanonicalForm(double xn);
    double processCanonicalFormTransposed(double xn);

private:
    AudioParametersPtr _parameters = std::make_shared<AudioParameters>();
    double _coeffs[BiquadCoefficients::NUM_COEFFS] = { 0.0, 0.0, 0.0, 0.0, 0.0,};
    double _delay[BiquadStateRegister::NUM_STATES] = { 0.0, 0.0, 0.0, 0.0};
    double _sampleRate = DEFAULT_SAMPLERATE;
    BiquadAlgorithm _biquadAlgorithm = BiquadAlgorithm::DirectForm;

};
