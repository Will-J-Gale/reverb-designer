#include <dsp/CombFilter.h>
#include <cmath>

CombFilter::CombFilter()
{
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

AudioParametersPtr CombFilter::getParameters()
{
    return parameters;
}

void CombFilter::onParametersChanged()
{
    double delayTimeMs = parameters->getParameterValueByName<double>("DelayTimeMs");
    double RT60TimeInMs = parameters->getParameterValueByName<double>("RT60 Ms");
    delayParameters->setParameterValueByName<DoubleParameter, double>("DelayTimeMs", delayTimeMs);

    double exponent = -3.0 * delayTimeMs * (1.0 / sampleRate);
    double rt60InSeconds = RT60TimeInMs / 1000.0;
    combG = std::pow(10, exponent / rt60InSeconds);
}