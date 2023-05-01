#pragma once

#include <dsp/Parameters.h>
#include <parameters/AudioParameters.h>
#include <dsp/IAudioProcessor.h>

class BiquadFilter : IAudioProcessor
{
public:
    BiquadFilter();
    ~BiquadFilter();

    virtual bool reset(double sampleRate) override;
    double process(double xn) override;
    virtual bool canProcessAudioFrame() { return false; };
    virtual AudioParametersPtr getParameters() override;

    void setCoefficients(double* coeffs);
    void setAlgorithm(BiquadAlgorithm algorithm);

private:
    AudioParametersPtr parameters = std::make_shared<AudioParameters>();
    double coeffs[numCoeffs] = { 0.0, 0.0, 0.0, 0.0, 0.0,};
    double delay[numStates] = { 0.0, 0.0, 0.0, 0.0};

    double sampleRate;
    BiquadAlgorithm biquadAlgorithm = BiquadAlgorithm::DirectForm;

    //Algorithms
    double processDirectForm(double xn);
    double processDirectFormTransposed(double xn);
    double processCanonicalForm(double xn);
    double processCanonicalFormTransposed(double xn);
};
