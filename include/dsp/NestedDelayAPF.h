/*
  ==============================================================================

    NestedDelayAPF.h
    Created: 3 Sep 2020 7:40:59pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include <dsp/DelayAPF.h>
#include <dsp/Parameters.h>

class NestedDelayAPF : public DelayAPF
{
public:
    void reset(double sampleRate, double maxDelayTimeInSeconds);
    double process(double xn);

    NestedDelayAPFParameters getParameters();
    void setParameters(NestedDelayAPFParameters newParameters);
private:
    NestedDelayAPFParameters parameters;
    DelayAPF nestedApf;
};