#include <dsp/NestedDelayAPF.h>

NestedDelayAPF::NestedDelayAPF()
{
    _parameters = MAKE_PARAMETERS({
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

    _parameters->addOnChangeCallback(std::bind(&NestedDelayAPF::onParametersChanged, this));
    onParametersChanged();
}

void NestedDelayAPF::reset(double sampleRate, double maxDelayTimeInSeconds)
{
    _nestedApf.reset(sampleRate);
}

double NestedDelayAPF::process(double xn)
{
    double delayedSample = 0.0;
    double g = _parameters->getParameterValueByName<double>("Outer APF G");
    double lpfG = _parameters->getParameterValueByName<double>("LPF G");
    bool enableLFO = _parameters->getParameterValueByName<bool>("Enable LFO");
    bool enableLPF = _parameters->getParameterValueByName< bool>("Enable LPF");

    if (enableLFO)
    {
        double lfoDepth = _parameters->getParameterValueByName<double>("LFO_DEPTH");
        double outerDelayTimeInMs = _parameters->getParameterValueByName<double>("Outer Delay Time");
        double lfoMaxModulationInMs = _parameters->getParameterValueByName<double>("LFO Max Modulation");
        SignalGenData lfoOutput = _lfo.renderAudioOutput();

        double maxDelay = outerDelayTimeInMs;
        double minDelay = maxDelay - lfoMaxModulationInMs;

        minDelay = fmax(minDelay, 0.0);

        double uniPolarLfo = Math::bipolarToUnipolar(lfoDepth * lfoOutput.normalOutput);
        double modDelayInMs = Math::uniPolarScale(uniPolarLfo, minDelay, maxDelay);

        delayedSample = _delay.readDelayAtTimeInMs(modDelayInMs);
    }
    else
    {
        delayedSample = _delay.readDelay();
    }

    double wn = xn + (delayedSample * g);

    if (enableLPF)
    {
        wn = (wn * (1 - lpfG)) + (_lpfState * lpfG);
        _lpfState = wn;
    }

    double ynInner = _nestedApf.process(wn);
    _delay.write(ynInner);

    double yn = delayedSample + (-g * wn);
    return yn;
}

void NestedDelayAPF::onParametersChanged()
{
    double innerDelayTimeInMs = _parameters->getParameterValueByName<double>("Inner Delay Time");
    double innerAPFG = _parameters->getParameterValueByName<double>("Inner APF G");

    AudioParametersPtr nestedAPFParameters = _nestedApf.getParameters();
    nestedAPFParameters->setParameterValueByName<double>("TimeMs", innerDelayTimeInMs);
    nestedAPFParameters->setParameterValueByName<double>("APF G", innerAPFG);
}