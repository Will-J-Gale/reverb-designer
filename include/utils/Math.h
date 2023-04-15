/*
  ==============================================================================

    Utilities.h
    Created: 29 Aug 2020 2:30:50pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include <cmath>

namespace Math
{
    const double PI = 3.14159265358979323846;

    inline double linearInterpolation(double y1, double y2, double fraction)
    {
        if (fraction > 1.0) { return y2; }

        return (fraction * y2)+ ((1 - fraction) * y1);
    }

    inline void clamp(double& value, double min, double max)
    {
        value = fmin(value, max);
        value = fmax(value, min);
    }

    inline void uniPolarClamp(double& value)
    {
        value = fmin(value, 1.0);
        value = fmax(value, 0.0);
    }

    inline double unipolarToBipolar(double value)
    {
        return (2.0 * value) - 1;
    }
    inline double bipolarToUnipolar(double value)
    {
        return (value + 1) / 2.0;
    }

    inline double uniPolarScale(double value, double min, double max)
    {
        Math::uniPolarClamp(value);
        return min + ((max - min) * value);
    }

    inline double bipolarScale(double value, double min, double max)
    {
        Math::clamp(value, -1.0, 1.0);

        double halfRange = (max - min) / 2.0;
        double midpoint = min + halfRange;

        return (value * halfRange) + midpoint;
    }
}
