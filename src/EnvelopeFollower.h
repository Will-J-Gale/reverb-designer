/*
  ==============================================================================

    EnvelopeFollower.h
    Created: 30 Aug 2020 1:40:50pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include "Parameters.h"
#include "AudioDetector.h"

/*
    This class uses an AudioDetector to modulate between 
    a generic min/max so it can be used anywhere

    It's process function just returns the value between
    outputMin and outputMax
*/
class EnvelopeFollower
{
public:

    EnvelopeFollowerParameters getParameters();
    void setParameters(EnvelopeFollowerParameters newParameters);
    bool reset(double sampleRate);
    double process(double xn);

private:
    EnvelopeFollowerParameters parameters;
    AudioDetector detector;
};