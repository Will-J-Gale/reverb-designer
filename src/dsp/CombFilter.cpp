#include <dsp/CombFilter.h>
#include <cmath>

CombFilter::CombFilter()
{
    _parameters = MAKE_PARAMETERS({
        std::make_shared<DoubleParameter>("TimeMs", 0.0, 0.0, 1000.0),
        std::make_shared<DoubleParameter>("RT60 Ms", 0.0, 0.0, 1000.0),
        std::make_shared<BooleanParameter>("Enable LPF", false),
        std::make_shared<DoubleParameter>("LPF G", 0.0, 0.0, 1.0)
    });

    _parameters->addOnChangeCallback(std::bind(&CombFilter::onParametersChanged, this));
    _delayParameters = _delay.getParameters();
    onParametersChanged();
}

void CombFilter::reset(double sampleRate)
{
    _sampleRate = sampleRate;
    _delay.reset(sampleRate);
    onParametersChanged();
    _lpfState = 0.0;
}

double CombFilter::process(double xn)
{
    double yn = _delay.readDelay();
    double input = 0.0;
    bool enableLPF = _parameters->getParameterValueByName<bool>("Enable LPF");

    if (enableLPF)
    {
        double lpfG = _parameters->getParameterValueByName<double>("LPF G");
        double g2 = lpfG * (1.0 - _combG);
        double filteredSignal = yn + (_lpfState * g2);
        input = xn + (filteredSignal * _combG);
        _lpfState = filteredSignal;
    }
    else
    {
        input = xn + (yn * _combG);
    }

    _delay.write(input);

    return yn;
}

bool CombFilter::canProcessAudioFrame()
{
    return false;
}

void CombFilter::onParametersChanged()
{
    double delayTimeMs = _parameters->getParameterValueByName<double>("TimeMs");
    double RT60TimeInMs = _parameters->getParameterValueByName<double>("RT60 Ms");
    _delayParameters->setParameterValueByName<double>("TimeMs", delayTimeMs);

    double exponent = -3.0 * delayTimeMs * (1.0 / _sampleRate);
    double rt60InSeconds = RT60TimeInMs / 1000.0;
    _combG = std::pow(10, exponent / rt60InSeconds);
}