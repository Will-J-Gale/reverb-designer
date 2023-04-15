/*
  ==============================================================================

    BiquadFilter.h
    Created: 22 Aug 2020 12:59:13pm
    Author:  Will

  ==============================================================================
*/

#pragma once

#include "Parameters.h"
#include "IAudioProcessor.h"

class BiquadFilter : IAudioProcessor
{
public:
    BiquadFilter();
    ~BiquadFilter();

    virtual bool reset(double sampleRate) override;
    double process(double xn) override;
    virtual bool canProcessAudioFrame() { return false; };

    void setCoefficients(double* coeffs);
    void setAlgorithm(BiquadAlgorithm algorithm);

private:
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
