#include <dsp/CombFilter.h>
#include <cmath>

CombFilter::CombFilter()
{
    parameters = MAKE_PARAMETERS({
        std::make_shared<DoubleParameter>("TimeMs", 0.0, 0.0, 1000.0),
        std::make_shared<DoubleParameter>("RT60 Ms", 0.0, 0.0, 1000.0),
        std::make_shared<BooleanParameter>("Enable LPF", false),
        std::make_shared<DoubleParameter>("LPF G", 0.0, 0.0, 1.0)
    });

    parameters->addOnChangeCallback(std::bind(&CombFilter::onParametersChanged, this));
    delayParameters = delay.getParameters();
    onParametersChanged();
}

bool CombFilter::reset(double sampleRate)
{
    this->sampleRate = sampleRate;
    delay.reset(sampleRate);
    onParametersChanged();
    lpfState = 0.0;
}

double CombFilter::process(double xn)
{
    double yn = delay.readDelay();
    double input = 0.0;
    bool enableLPF = parameters->getParameterValueByName<bool>("Enable LPF");

    if (enableLPF)
    {
        double lpfG = parameters->getParameterValueByName<double>("LPF G");
        double g2 = lpfG * (1.0 - combG);
        double filteredSignal = yn + (lpfState * g2);
        input = xn + (filteredSignal * combG);
        lpfState = filteredSignal;
    }
    else
    {
        input = xn + (yn * combG);
    }

    delay.write(input);

    return yn;
}

bool CombFilter::canProcessAudioFrame()
{
    return false;
}

void CombFilter::onParametersChanged()
{
    double delayTimeMs = parameters->getParameterValueByName<double>("TimeMs");
    double RT60TimeInMs = parameters->getParameterValueByName<double>("RT60 Ms");
    delayParameters->setParameterValueByName<double>("TimeMs", delayTimeMs);

    double exponent = -3.0 * delayTimeMs * (1.0 / sampleRate);
    double rt60InSeconds = RT60TimeInMs / 1000.0;
    combG = std::pow(10, exponent / rt60InSeconds);
}