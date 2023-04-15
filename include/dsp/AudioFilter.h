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

class AudioFilter : public IAudioProcessor
{
public:
    virtual bool reset(double sampleRate);
    virtual double process(double xn);
    virtual bool canProcessAudioFrame();
    virtual void setSampleRate(double sampleRate);
    void setParameters(AudioFilterParameters newParameters);
    AudioFilterParameters getParameters();
    void calculateCoefficients();
private:
    BiquadFilter biquad;
    AudioFilterParameters parameters;

    double sampleRate;

    double coeffs[numCoeffs] = { 0.0, 0.0, 0.0, 0.0, 0.0};

    //Filter coefficient calculations
    void calculateLPF1();
    void calculateLPF2();
    void calculateLPFButterworth();
    void calculateHPF1();
    void calculateHPF2();
    void calculateHPFButterworth();
    void calculateBPF();
    void calculateBSF();
    void calculateAPF();
    void calculateHSF();
    void calculateLSF();
    void calculatePEQ();
    void calculatePEQConstant();
    void calculateGEQ();
    void calcualteLWRLPF2();
    void calcualteLWRHPF2();
};
