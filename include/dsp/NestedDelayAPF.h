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
    AudioParametersPtr getParameters() override;
    void onParametersChanged();

private:
    AudioParametersPtr parameters = MAKE_PARAMETERS({
        std::make_shared<DoubleParameter>("Outer Delay Time", 0.0, APF_MIN_DELAY, APF_MAX_DELAY),
        std::make_shared<DoubleParameter>("Outer APF G", 0.0, 0.0, 1.0),
        std::make_shared<DoubleParameter>("Inner Delay Time", 0.0, APF_MIN_DELAY, APF_MAX_DELAY),
        std::make_shared<DoubleParameter>("Inner APF G", 0.0, 0.0, 1.0),
        std::make_shared<BooleanParameter>("Enable LPF", false),
        std::make_shared<DoubleParameter>("LPF G", 0.0, 0.0, 1.0),
        std::make_shared<BooleanParameter>("Enable LFO", false),
        std::make_shared<DoubleParameter>("LFO Rate HZ", 0.0, APF_LFO_RATE_MIN, APF_LFO_RATE_MAX),
        std::make_shared<DoubleParameter>("LFO Depth", 0.0, 0.0, 1.0),
        std::make_shared<DoubleParameter>("LFO Max Modulation", 0.0, APF_MIN_MOD_FREQ, APF_MAX_MOD_FREQ),
    });

    DelayAPF nestedApf;
};