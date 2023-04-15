/*
  ==============================================================================

    AudioFilter.cpp
    Created: 22 Aug 2020 2:02:10pm
    Author:  Will

  ==============================================================================
*/
#pragma once
#include <cmath>
#include <dsp/AudioFilter.h>
#include <dsp/AudioFilterCalculations.h>
#include <utils/Math.h>

bool AudioFilter::reset(double sampleRate)
{
    this->sampleRate = sampleRate;
    biquad.reset(sampleRate);
    setParameters(parameters);

	return true;
}

double AudioFilter::process(double xn)
{
    return (coeffs[D0] * xn) + (coeffs[C0] * biquad.process(xn));
}

bool AudioFilter::canProcessAudioFrame()
{
	return false;
}

void AudioFilter::setSampleRate(double newSampleRate)
{
    sampleRate = newSampleRate;
}

void AudioFilter::setParameters(AudioFilterParameters newParameters)
{
    parameters = newParameters;

    if (parameters.Q <= 0)
    {
        parameters.Q = DEFAULT_Q;
    }

    calculateCoefficients();
}

AudioFilterParameters AudioFilter::getParameters()
{
    return parameters;
}

void AudioFilter::calculateCoefficients()
{
    double fc = parameters.fc;
    double fs = sampleRate;
    double Q = parameters.Q;
    double gain_dB = parameters.gain_dB;

    switch (parameters.filterType)
    {
        case FilterType::LPF1:
            AudioFilterCalculations::calculateLPF1(coeffs, fc, fs, Q);
            break;

        case FilterType::LPF2:
            AudioFilterCalculations::calculateLPF2(coeffs, fc, fs, Q);
            break;

        case FilterType::LPFButterworth:
            AudioFilterCalculations::calculateLPFButterworth(coeffs, fc, fs, Q);
            break;

        case FilterType::HPF1:
            AudioFilterCalculations::calculateHPF1(coeffs, fc, fs, Q);
            break;

        case FilterType::HPF2:
            AudioFilterCalculations::calculateHPF2(coeffs, fc, fs, Q);
            break;

        case FilterType::HPFButterworth:
            AudioFilterCalculations::calculateHPFButterworth(coeffs, fc, fs, Q);
            break;

        case FilterType::BPF:
            AudioFilterCalculations::calculateBPF(coeffs, fc, fs, Q);
            break;

        case FilterType::BSF:
            AudioFilterCalculations::calculateBSF(coeffs, fc, fs, Q);
            break;

        case FilterType::APF:
            AudioFilterCalculations::calculateAPF(coeffs, fc, fs, Q);
            break;

        case FilterType::HSF:
            AudioFilterCalculations::calculateHSF(coeffs, fc, fs, Q, gain_dB);
            break;

        case FilterType::LSF:
            AudioFilterCalculations::calculateLSF(coeffs, fc, fs, Q, gain_dB);
            break;

        case FilterType::PEQ:
            AudioFilterCalculations::calculatePEQ(coeffs, fc, fs, Q, gain_dB);
            break;
        
        case FilterType::PEQConstant:
            AudioFilterCalculations::calculatePEQConstant(coeffs, fc, fs, Q, gain_dB);
            break;

        case FilterType::GEQ:
            AudioFilterCalculations::calculateGEQ(coeffs, fc, fs, Q, gain_dB);
            break;

        case FilterType::LWRLPF2:
            AudioFilterCalculations::calcualteLWRLPF2(coeffs, fc, fs);
            break;
        case FilterType::LWRHPF2:
            AudioFilterCalculations::calcualteLWRHPF2(coeffs, fc, fs);
            break;
        
        biquad.setCoefficients(coeffs);
    }
}