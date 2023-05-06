#include <dsp/Gain.h>

Gain::Gain()
{
    parameters = MAKE_PARAMETERS({
        std::make_shared<DoubleParameter>("Gain", 1.0, 0.0, 1.0),
    });

    parameters->addOnChangeCallback(std::bind(&Gain::onParametersChanged, this));
    onParametersChanged();
}

bool Gain::reset(double sampleRate)
{
    return false;
}

double Gain::process(double xn)
{
    double gain = parameters->getParameterValueByName<double>("Gain");
    return xn * gain;
}

bool Gain::canProcessAudioFrame()
{
    return false;
}