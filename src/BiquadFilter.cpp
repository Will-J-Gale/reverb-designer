/*
  ==============================================================================

    BiquadFilter.cpp
    Created: 22 Aug 2020 12:59:13pm
    Author:  Will

  ==============================================================================
*/

#include <dsp/BiquadFilter.h>
#include <cstring>

BiquadFilter::BiquadFilter()
{
}

BiquadFilter::~BiquadFilter()
{
}

bool BiquadFilter::reset(double sampleRate)
{
    memset(&delay[0], 0, numStates);
    return true;
}

double BiquadFilter::process(double xn)
{
    if (biquadAlgorithm == BiquadAlgorithm::DirectForm)
    {
        return processDirectForm(xn);
    }
    else if (biquadAlgorithm == BiquadAlgorithm::DirectFormTransposed)
    {
        return processDirectFormTransposed(xn);
    }
    else if (biquadAlgorithm == BiquadAlgorithm::CanonicalForm)
    {
        return processCanonicalForm(xn);
    }
    else if (biquadAlgorithm == BiquadAlgorithm::CanonicalFormTransposed)
    {
        return processCanonicalFormTransposed(xn);
    }
}

void BiquadFilter::setCoefficients(double* coeffs)
{
    std::memcpy(&this->coeffs[0], &coeffs[0], sizeof(double)*numCoeffs);
}

void BiquadFilter::setAlgorithm(BiquadAlgorithm algorithm)
{
    biquadAlgorithm = algorithm;
}

double BiquadFilter::processDirectForm(double xn)
{
    double yn = (xn * coeffs[A0])
        + (delay[x_z1] * coeffs[A1])
        + (delay[x_z2] * coeffs[A2])
        - (delay[y_z1] * coeffs[B1])
        - (delay[y_z2] * coeffs[B2]);

    delay[x_z1] = xn;
    delay[x_z2] = delay[x_z1];
    delay[y_z1] = yn;
    delay[y_z2] = delay[y_z1];

    return yn;
}

double BiquadFilter::processDirectFormTransposed(double xn)
{
    return 0.0;
}

double BiquadFilter::processCanonicalForm(double xn)
{
    return 0.0;
}

double BiquadFilter::processCanonicalFormTransposed(double xn)
{
    return 0.0;
}


