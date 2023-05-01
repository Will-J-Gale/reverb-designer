#include <dsp/DelayAPF.h>

DelayAPF::DelayAPF()
{
    parameters->addOnChangeCallback(std::bind(&DelayAPF::onParametersChanged, this));
    delayParameters = delay.getParameters();

    onParametersChanged();
}

bool DelayAPF::reset(double sampleRate)
{
    this->sampleRate = sampleRate;
    delay.reset(sampleRate);
    lfo.reset(sampleRate);

    delayParameters = delay.getParameters();
}

double DelayAPF::process(double xn)
{
    double g = parameters->getParameterValueByName<double>("APF G");
    double g2 = parameters->getParameterValueByName<double>("LPF G");
    double lfoDepth = parameters->getParameterValueByName<double>("LFO Depth");
    double delayTimeMs = parameters->getParameterValueByName<double>("DelayTimeMs");
    double lfoMaxModulationInMs = parameters->getParameterValueByName<double>("LFO Max Modulation ms");
    double enableLPF = parameters->getParameterValueByName<bool>("Enable LPF");
    double enableLFO = parameters->getParameterValueByName<bool>("Enable LFO");
    double delayedSample = 0.0;

    if (enableLFO)
    {
        SignalGenData lfoOutput = lfo.renderAudioOutput();
        double lfoDepth = lfoDepth;

        double maxDelay = delayTimeMs;
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
    double yn = (wn * -g) + delayedSample;

    /*
        This is different to the book's version
        The book does the filter on delayedSample
        I don't think it matters where the filter is
        because either the delay will already have filtering
        applied before it has been read, or after it has been read,
        resulting in xn always being added to a filtered version
    */

    if (enableLPF)
    {
        wn = (wn * (1 - g2)) + (lpfState * g2);
        lpfState = wn;
    }

    delay.write(wn);

    return yn;
}

bool DelayAPF::canProcessAudioFrame()
{
    return false;
}

AudioParametersPtr DelayAPF::getParameters()
{
    return parameters;
}

void DelayAPF::onParametersChanged()
{
    double delayTimeInMs = parameters->getParameterValueByName<double>("DelayTimeMs");
    double lfoRate = parameters->getParameterValueByName<double>("LFO Rate");
    
    AudioParametersPtr delayParameters = delay.getParameters();
    delayParameters->setParameterValueByName<DoubleParameter, double>("DelayTimeMs", delayTimeInMs);

    OscillatorParameters lfoPrams = lfo.getParameters();
    lfoPrams.frequency = lfoRate;
    lfo.setParameters(lfoPrams);
}