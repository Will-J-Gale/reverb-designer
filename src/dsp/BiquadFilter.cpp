#include <dsp/BiquadFilter.h>
#include <cstring>

BiquadFilter::BiquadFilter()
{
}

BiquadFilter::~BiquadFilter()
{
}

void BiquadFilter::reset(double sampleRate)
{
    memset(&_delay[0], 0, NUM_STATES);
}

double BiquadFilter::process(double xn)
{
    if (_biquadAlgorithm == BiquadAlgorithm::DirectForm)
    {
        return processDirectForm(xn);
    }
    else if (_biquadAlgorithm == BiquadAlgorithm::DirectFormTransposed)
    {
        return processDirectFormTransposed(xn);
    }
    else if (_biquadAlgorithm == BiquadAlgorithm::CanonicalForm)
    {
        return processCanonicalForm(xn);
    }
    else
    {
        return processCanonicalFormTransposed(xn);
    }
}

void BiquadFilter::setCoefficients(double* coeffs)
{
    std::memcpy(&_coeffs[0], &coeffs[0], sizeof(double)*NUM_COEFFS);
}

void BiquadFilter::setAlgorithm(BiquadAlgorithm algorithm)
{
    _biquadAlgorithm = algorithm;
}

double BiquadFilter::processDirectForm(double xn)
{
    double yn = (xn * _coeffs[A0])
        + (_delay[x_z1] * _coeffs[A1])
        + (_delay[x_z2] * _coeffs[A2])
        - (_delay[y_z1] * _coeffs[B1])
        - (_delay[y_z2] * _coeffs[B2]);

    _delay[x_z1] = xn;
    _delay[x_z2] = _delay[x_z1];
    _delay[y_z1] = yn;
    _delay[y_z2] = _delay[y_z1];

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

AudioParametersPtr BiquadFilter::getParameters()
{
    return _parameters;
}