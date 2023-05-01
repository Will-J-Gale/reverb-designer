#include <dsp/NestedDelayAPF.h>

NestedDelayAPF::NestedDelayAPF()
{
    parameters->addOnChangeCallback(std::bind(&NestedDelayAPF::onParametersChanged, this));
    onParametersChanged();
}

void NestedDelayAPF::reset(double sampleRate, double maxDelayTimeInSeconds)
{
    nestedApf.reset(sampleRate);
}

double NestedDelayAPF::process(double xn)
{
    double delayedSample = 0.0;
    double g = parameters->getParameterValueByName<double>("Outer APF G");
    double lpfG = parameters->getParameterValueByName<double>("LPF G");
    bool enableLFO = parameters->getParameterValueByName<bool>("Enable LFO");
    bool enableLPF = parameters->getParameterValueByName< bool>("Enable LPF");

    if (enableLFO)
    {
        double lfoDepth = parameters->getParameterValueByName<double>("LFO_DEPTH");
        double outerDelayTimeInMs = parameters->getParameterValueByName<double>("Outer Delay Time");
        double lfoMaxModulationInMs = parameters->getParameterValueByName<double>("LFO Max Modulation");
        SignalGenData lfoOutput = lfo.renderAudioOutput();

        double maxDelay = outerDelayTimeInMs;
        double minDelay = maxDelay - lfoMaxModulationInMs;

        minDelay = fmax(minDelay, 0.0);

        double uniPolarLfo = Math::bipolarToUnipolar(lfoDepth * lfoOutput.normalOutput);
        double modDelayInMs = Math::uniPolarScale(uniPolarLfo, minDelay, maxDelay);

        delayedSample = delay.readDelayAtTimeInMs(modDelayInMs);
    }
    else
    {
        delayedSample = delay.readDelay();
    }

    double wn = xn + (delayedSample * g);

    if (enableLPF)
    {
        wn = (wn * (1 - lpfG)) + (lpfState * lpfG);
        lpfState = wn;
    }

    double ynInner = nestedApf.process(wn);
    delay.write(ynInner);

    double yn = delayedSample + (-g * wn);
    return yn;
}

AudioParametersPtr NestedDelayAPF::getParameters()
{
    return parameters;
}

void NestedDelayAPF::onParametersChanged()
{
    double innerDelayTimeInMs = parameters->getParameterValueByName<double>("Inner Delay Time");
    double innerAPFG = parameters->getParameterValueByName<double>("Inner APF G");

    AudioParametersPtr nestedAPFParameters = nestedApf.getParameters();
    nestedAPFParameters->setParameterValueByName<DoubleParameter, double>("Inner Delay Time", innerDelayTimeInMs);
    nestedAPFParameters->setParameterValueByName<DoubleParameter, double>("Inner APF G", innerAPFG);
}