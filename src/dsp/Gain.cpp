#include <dsp/Gain.h>

Gain::Gain()
{
    _parameters = MAKE_PARAMETERS({
        std::make_shared<DoubleParameter>("Gain", 1.0, -1.0, 1.0),
    });

    _parameters->addOnChangeCallback(std::bind(&Gain::onParametersChanged, this));
    onParametersChanged();
}

void Gain::reset(double sampleRate)
{
}

double Gain::process(double xn)
{
    double gain = _parameters->getParameterValueByName<double>("Gain");
    return xn * gain;
}

bool Gain::canProcessAudioFrame()
{
    return false;
}