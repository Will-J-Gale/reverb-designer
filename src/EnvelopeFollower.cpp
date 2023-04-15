/*
  ==============================================================================

    EnvelopeFollower.cpp
    Created: 30 Aug 2020 1:40:50pm
    Author:  Will

  ==============================================================================
*/
#include <cmath>
#include "EnvelopeFollower.h"
#include "Utilities.h"

EnvelopeFollowerParameters EnvelopeFollower::getParameters()
{
    return parameters;
}

void EnvelopeFollower::setParameters(EnvelopeFollowerParameters newParameters)
{
    parameters = newParameters;

    AudioDetectorParameters detectorParams = detector.getParameters();
    detectorParams.attackTimeInMs = parameters.attackTimeInMs;
    detectorParams.releaseTimeInMs = parameters.releaseTimeInMs;
    detector.setParameters(detectorParams);
}

bool EnvelopeFollower::reset(double sampleRate)
{
    detector.reset(sampleRate);
    return true;
}

double EnvelopeFollower::process(double xn)
{
    double threshValue = pow(10.0, parameters.thresholdDb / 20.0);

    double detectDb = detector.process(xn);
    double detectValue = pow(10.0, detectDb / 20.0);
    double deltaValue = detectValue / threshValue;

    double controlValue = parameters.outputMin;

    if (deltaValue > 0)
    {
        double modulatorValue = deltaValue * parameters.sensitivity;
        controlValue = uniPolarScale(
            modulatorValue,
            parameters.outputMin,
            parameters.outputMax
        );
    }

    return controlValue;
}
