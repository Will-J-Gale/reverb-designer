#pragma once
#include <dsp/DelayAPF.h>
#include <dsp/Parameters.h>
#include <parameters/Parameter.h>
#include <parameters/AudioParameters.h>
#include <utils/Constants.h>

class NestedDelayAPF : public DelayAPF
{
public:
    NestedDelayAPF();
    void reset(double sampleRate, double maxDelayTimeInSeconds);
    double process(double xn);
    void onParametersChanged();

private:
    DelayAPF _nestedApf;
};