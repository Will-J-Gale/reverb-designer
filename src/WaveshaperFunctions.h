/*
  ==============================================================================

    WaveshaperFunctions.h
    Created: 7 Sep 2020 2:32:44pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include <cmath>

inline double sign(double x)
{
    return (x > 0.0) - (x < 0.0);
}

//Calcualtes the saturaiton value depending on how much asymmetry you want
inline double calcWSGain(double xn, double saturation, double asymmetry)
{
    double g = ((xn >= 0.0 && asymmetry > 0.0) || (xn < 0.0 && asymmetry < 0.0)) ? saturation * (1.0 + 4.0 * fabs(asymmetry)) : saturation;
    return g;
}

inline double atanWaveshaper(double xn, double saturation)
{
    return atan(xn * saturation) / atan(saturation);
}

inline double tanhWaveshaper(double xn, double saturation)
{
    return tanh(xn * saturation) / tanh(saturation);
}

inline double softClipWaveshaper(double xn, double saturation)
{
    //Remember to use 1.0 instead of 1
    //Using 1 causes quantization distortion
    //due to integer math.
    return sign(xn) * (1.0 - exp(-fabs(saturation * xn)));
}

inline double fuzzExp1Waveshaper(double xn, double saturation, double asymmetry)
{
    double wsGain = calcWSGain(xn, saturation, asymmetry);
    return sign(xn) * ((1.0 - exp(-fabs(wsGain * xn))) / (1.0 - exp(-wsGain)));
}