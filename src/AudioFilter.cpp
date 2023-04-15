/*
  ==============================================================================

    AudioFilter.cpp
    Created: 22 Aug 2020 2:02:10pm
    Author:  Will

  ==============================================================================
*/
#pragma once
#include <cmath>
#include "AudioFilter.h"
#include "Utilities.h"

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
    switch (parameters.filterType)
    {
        case FilterType::LPF1:
            calculateLPF1();
            break;

        case FilterType::LPF2:
            calculateLPF2();
            break;

        case FilterType::LPFButterworth:
            calculateLPFButterworth();
            break;

        case FilterType::HPF1:
            calculateHPF1();
            break;

        case FilterType::HPF2:
            calculateHPF2();
            break;

        case FilterType::HPFButterworth:
            calculateHPFButterworth();
            break;

        case FilterType::BPF:
            calculateBPF();
            break;

        case FilterType::BSF:
            calculateBSF();
            break;

        case FilterType::APF:
            calculateAPF();
            break;

        case FilterType::HSF:
            calculateHSF();
            break;

        case FilterType::LSF:
            calculateLSF();
            break;

        case FilterType::PEQ:
            calculatePEQ();
            break;
        
        case FilterType::PEQConstant:
            calculatePEQConstant();
            break;

        case FilterType::GEQ:
            calculateGEQ();
            break;

        case FilterType::LWRLPF2:
            calcualteLWRLPF2();
            break;
        case FilterType::LWRHPF2:
            calcualteLWRHPF2();
            break;
    }
}

//Filter coefficient calculations
void AudioFilter::calculateLPF1()
{
    double fc = parameters.fc;
    double fs = sampleRate;
    double Q = parameters.Q;

    double theta = (2.0 * PI * fc) / fs;
    double gamma = cos(theta) / (1.0 + sin(theta));

    coeffs[A0] = (1.0 - gamma) / 2.0;
    coeffs[A1] = (1.0 - gamma) / 2.0;
    coeffs[A2] = 0.0;
    coeffs[B1] = -gamma;
    coeffs[B2] = 0.0;
    coeffs[C0] = 1.0;
    coeffs[D0] = 0.0;

    biquad.setCoefficients(coeffs);
}

void AudioFilter::calculateLPF2()
{
    double fc = parameters.fc;
    double fs = sampleRate;
    double Q = parameters.Q;

    double theta = (2.0 * PI * fc) / fs;
    double d = 1.0 / Q;

    double sinThetha = sin(theta);
    double dSinTheta = (d / 2.0) * (sinThetha);
    double beta = 0.5 * ((1.0 - dSinTheta) / (1.0 + dSinTheta));
    double gamma = (0.5 + beta) * cos(theta);

    coeffs[A0] = (0.5 + beta - gamma) / 2.0;
    coeffs[A1] = 0.5 + beta - gamma;
    coeffs[A2] = (0.5 + beta - gamma) / 2.0;;
    coeffs[B1] = -2.0 * gamma;
    coeffs[B2] = 2.0 * beta;
    coeffs[C0] = 1.0;
    coeffs[D0] = 0.0;

    biquad.setCoefficients(coeffs);
}

void AudioFilter::calculateLPFButterworth()
{
    double fc = parameters.fc;
    double fs = sampleRate;
    double Q = parameters.Q;

    double C = 1.0/ tan((PI * fc) / fs);
    double C2 = C * C;

    double a0 = 1.0/ (1.0+ (sqrt(2) * C) + C2);
    coeffs[A0] = a0;
    coeffs[A1] = 2.0* a0;
    coeffs[A2] = a0;
    coeffs[B1] = (2.0* a0) * (1.0- C2);
    coeffs[B2] = a0 * (1.0- (sqrt(2) * C) + C2);
    coeffs[C0] = 1.0;
    coeffs[D0] = 0.0;

    biquad.setCoefficients(coeffs);
}

void AudioFilter::calculateHPF1()
{
    double fc = parameters.fc;
    double fs = sampleRate;
    double Q = parameters.Q;

    double theta = (2.0 * PI * fc) / fs;
    double gamma = cos(theta) / (1.0+ sin(theta));

    coeffs[A0] = (1.0+ gamma) / 2;
    coeffs[A1] = -((1.0+ gamma) / 2);
    coeffs[A2] = 0.0;
    coeffs[B1] = -gamma;
    coeffs[B2] = 0.0;
    coeffs[C0] = 1.0;
    coeffs[D0] = 0.0;

    biquad.setCoefficients(coeffs);
}

void AudioFilter::calculateHPF2()
{
    double fc = parameters.fc;
    double fs = sampleRate;
    double Q = parameters.Q;

    double theta = (2.0 * PI * fc) / fs;
    double d = 1.0/ Q;

    double sinThetha = sin(theta);
    double dSinTheta = (d / 2) * (sinThetha);
    double beta = 0.5 * ((1.0- dSinTheta) / (1.0+ dSinTheta));
    double gamma = (0.5 + beta) * cos(theta);

    coeffs[A0] = (0.5 + beta + gamma) / 2.0;
    coeffs[A1] = -(0.5 + beta + gamma);
    coeffs[A2] = (0.5 + beta + gamma) / 2.0;;
    coeffs[B1] = -2.0 * gamma;
    coeffs[B2] = 2.0 * beta;
    coeffs[C0] = 1.0;
    coeffs[D0] = 0.0;

    biquad.setCoefficients(coeffs);
}

void AudioFilter::calculateHPFButterworth()
{
    double fc = parameters.fc;
    double fs = sampleRate;
    double Q = parameters.Q;

    double C = 1.0/ tan((PI * fc) / fs);
    double C2 = C * C;

    double a0 = 1.0/ (1.0+ (sqrt(2) * C) + C2);
    coeffs[A0] = a0;
    coeffs[A1] = -2.0* a0;
    coeffs[A2] = a0;
    coeffs[B1] = (2.0* a0) * (C2 - 1);
    coeffs[B2] = a0 * (1.0- (sqrt(2) * C) + C2);
    coeffs[C0] = 1.0;
    coeffs[D0] = 0.0;

    biquad.setCoefficients(coeffs);
}

void AudioFilter::calculateBPF()
{
    double fc = parameters.fc;
    double fs = sampleRate;
    double Q = parameters.Q;

    double K = tan((PI * fc) / fs);
    double K2 = (K * K);

    double delta = (K2* Q) + K + Q;

    coeffs[A0] = K / delta;
    coeffs[A1] = 0.0;
    coeffs[A2] = -K / delta;
    coeffs[B1] = (2*Q) * (K2 - 1);
    coeffs[B2] = ((K2 * Q) - K + Q) / delta;
    coeffs[C0] = 1.0;
    coeffs[D0] = 0.0;

    biquad.setCoefficients(coeffs);
}

void AudioFilter::calculateBSF()
{
    double fc = parameters.fc;
    double fs = sampleRate;
    double Q = parameters.Q;

    double K = tan((PI * fc) / fs);
    double K2 = (K * K);

    double delta = (K2 * Q) + K + Q;

    coeffs[A0] = Q * (K2 + 1);
    coeffs[A1] = (2.0 * Q) * (K2 + 1);
    coeffs[A2] = (Q * (K2 + 1)) / delta;
    coeffs[B1] = (2.0 * Q) * (K2 - 1);
    coeffs[B2] = ((K2 * Q) - K + Q) / delta;
    coeffs[C0] = 1.0;
    coeffs[D0] = 0.0;

    biquad.setCoefficients(coeffs);
}

void AudioFilter::calculateAPF()
{
}

void AudioFilter::calculateHSF()
{
    double fc = parameters.fc;
    double fs = sampleRate;
    double Q = parameters.Q;
    double gain = parameters.gain_dB;

    double theta = (2.0 * PI * fc) / fs;
    double u = pow(10.0, gain / 20.0);
    double beta = (1.0+ u) / 4;
    double delta = beta * tan(theta / 2.0);
    double gamma = (1.0- delta) / (1.0+ delta);

    coeffs[A0] = (1.0+ gamma) / 2;
    coeffs[A1] = -((1.0+ gamma) / 2);
    coeffs[A2] = 0.0;
    coeffs[B1] = -gamma;
    coeffs[B2] = 0.0;
    coeffs[C0] = u - 1.0;
    coeffs[D0] = 1.0;

    biquad.setCoefficients(coeffs);
}

void AudioFilter::calculateLSF()
{
    double fc = parameters.fc;
    double fs = sampleRate;
    double Q = parameters.Q;
    double gain = parameters.gain_dB;

    double theta = (2.0 * PI * fc) / fs;
    double u = pow(10.0, gain / 20.0);
    double beta = 4 / (1.0+ u);
    double delta = beta * tan(theta / 2.0);
    double gamma = (1.0- delta) / (1.0+ delta);

    coeffs[A0] = (1.0- gamma) / 2;
    coeffs[A1] = (1.0- gamma) / 2;;
    coeffs[A2] = 0.0;
    coeffs[B1] = -gamma;
    coeffs[B2] = 0.0;
    coeffs[C0] = u - 1.0;
    coeffs[D0] = 1.0;

    biquad.setCoefficients(coeffs);
}

void AudioFilter::calculatePEQ()
{
    double fc = parameters.fc;
    double fs = sampleRate;
    double Q = parameters.Q;
    double gain = parameters.gain_dB;

    double theta = (2.0 * PI * fc) / fs;
    double u = pow(10.0, gain / 20.0);
    double zeta = 4.0 / (1.0+ u);

    double zetaTanTheta = zeta * tan(theta / (2.0* Q));

    double beta = 0.5 * ((1.0- zetaTanTheta) / (1.0+ zetaTanTheta));
    double gamma = (0.5 + beta) * cos(theta);

    coeffs[A0] = 0.5 - beta;
    coeffs[A1] = 0.0;
    coeffs[A2] = -(0.5 - beta);
    coeffs[B1] = -2.0 * gamma;
    coeffs[B2] = 2.0 * beta;
    coeffs[C0] = u - 1.0;
    coeffs[D0] = 1.0;

    biquad.setCoefficients(coeffs);
}

void AudioFilter::calculatePEQConstant()
{
    double fc = parameters.fc;
    double fs = sampleRate;
    double Q = parameters.Q;
    double gain = parameters.gain_dB;

    double K = tan((PI * fc) / fs);
    double K2 = K * K;
    double V0 = pow(10.0, gain / 20.0);
    double d0 = 1.0 + ((1.0/ Q) * K) + K2;
    double e0 = 1.0 + ((1.0/ (V0 * Q)) * K) + K2;

    double alpha = 1.0+ ((V0 / Q) * K) + K2;
    double beta = 2.0 * (K2 - 1.0);
    double gamma = 1.0 - ((V0 / Q) * K) + K2;
    double delta = 1.0 - ((1.0/ Q) * K) + K2;
    double eta = 1.0 - ((1.0/ (V0 * Q)) * K) + K2;

    double a0;
    double a1;
    double a2;
    double b1;
    double b2;

    if (gain >= 0)
    {
        a0 = alpha / d0;
        a1 = beta / d0;
        a2 = gamma / d0;
        b1 = beta / d0;
        b2 = delta / d0;
    }
    else
    {
        a0 = d0 / e0;
        a1 = beta / e0;
        a2 = delta / e0;
        b1 = beta / e0;
        b2 = eta / e0;
    }

    coeffs[A0] = a0;
    coeffs[A1] = a1;
    coeffs[A2] = a2;
    coeffs[B1] = b1;
    coeffs[B2] = b2;
    coeffs[C0] = 1.0;
    coeffs[D0] = 0.0;

    biquad.setCoefficients(coeffs);
}

void AudioFilter::calculateGEQ()
{
}

void AudioFilter::calcualteLWRLPF2()
{
    double fc = parameters.fc;
    double fs = sampleRate;

    double omega = PI * fc;
    double omega2 = omega * omega;
    double theta = omega / fs;
    double kappa = omega / tan(theta);
    double kappa2 = kappa * kappa;
    double delta = kappa2 + omega2 + (2 * kappa * omega);

    coeffs[A0] = omega2 / delta;
    coeffs[A1] = 2 * (omega2 / delta);
    coeffs[A2] = omega2 / delta;;
    coeffs[B1] = ((-2 * kappa2) + (2 * omega2)) / delta;
    coeffs[B2] = ((-2 * kappa * omega) + (kappa2) + (omega2)) / delta;
    coeffs[C0] = 1.0;
    coeffs[D0] = 0.0;

    biquad.setCoefficients(coeffs);
}

void AudioFilter::calcualteLWRHPF2()
{
    double fc = parameters.fc;
    double fs = sampleRate;

    double omega = PI * fc;
    double omega2 = omega * omega;
    double theta = omega / fs;
    double kappa = omega / tan(theta);
    double kappa2 = kappa * kappa;
    double delta = kappa2 + omega2 + (2 * kappa * omega);

    coeffs[A0] = kappa2 / delta;
    coeffs[A1] = (-2 * kappa2) / delta;
    coeffs[A2] = kappa2 / delta;
    coeffs[B1] = ((-2 * kappa2) + (2 * omega2)) / delta;
    coeffs[B2] = ((-2 * kappa * omega) + (kappa2)+(omega2)) / delta;
    coeffs[C0] = 1.0;
    coeffs[D0] = 0.0;

    biquad.setCoefficients(coeffs);
}
