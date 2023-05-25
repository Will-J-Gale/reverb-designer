#include <dsp/DelayAPF.h>

DelayAPF::DelayAPF()
{
    _parameters = MAKE_PARAMETERS({
        std::make_shared<DoubleParameter>("TimeMs", 0.0, APF_MIN_DELAY, APF_MAX_DELAY),
        std::make_shared<DoubleParameter>("APF G", 0.0, 0.0, 1.0),
        std::make_shared<BooleanParameter>("Enable LPF", false),
        std::make_shared<DoubleParameter>("LPF G", 0.0, 0.0, 1.0),
        std::make_shared<BooleanParameter>("Enable LFO", false),
        std::make_shared<DoubleParameter>("LFO Rate", 0.0, APF_LFO_RATE_MIN, APF_LFO_RATE_MAX),
        std::make_shared<DoubleParameter>("LFO Depth", 0.0, 0.0, 1.0),
        std::make_shared<DoubleParameter>("LFO Max Modulation ms", 0.0, APF_MIN_MOD_FREQ, APF_MAX_MOD_FREQ),
    });

    _parameters->addOnChangeCallback(std::bind(&DelayAPF::onParametersChanged, this));
    _delayParameters = _delay.getParameters();

    onParametersChanged();
}

void DelayAPF::reset(double sampleRate)
{
    _sampleRate = sampleRate;
    _delay.reset(sampleRate);
    _lfo.reset(sampleRate);
    _delayParameters = _delay.getParameters();
}

double DelayAPF::process(double xn)
{
    double g = _parameters->getParameterValueByName<double>("APF G");
    double g2 = _parameters->getParameterValueByName<double>("LPF G");
    double lfoDepth = _parameters->getParameterValueByName<double>("LFO Depth");
    double delayTimeMs = _parameters->getParameterValueByName<double>("TimeMs");
    double lfoMaxModulationInMs = _parameters->getParameterValueByName<double>("LFO Max Modulation ms");
    double enableLPF = _parameters->getParameterValueByName<bool>("Enable LPF");
    double enableLFO = _parameters->getParameterValueByName<bool>("Enable LFO");
    double delayedSample = 0.0;

    if (enableLFO)
    {
        SignalGenData lfoOutput = _lfo.renderAudioOutput();
        double maxDelay = delayTimeMs;
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
        wn = (wn * (1 - g2)) + (_lpfState * g2);
        _lpfState = wn;
    }

    _delay.write(wn);

    return yn;
}

bool DelayAPF::canProcessAudioFrame()
{
    return false;
}

void DelayAPF::onParametersChanged()
{
    double delayTimeInMs = _parameters->getParameterValueByName<double>("TimeMs");
    double lfoRate = _parameters->getParameterValueByName<double>("LFO Rate");
    
    AudioParametersPtr delayParameters = _delay.getParameters();
    delayParameters->setParameterValueByName<double>("TimeMs", delayTimeInMs);

    OscillatorParameters lfoPrams = _lfo.getParameters();
    lfoPrams.frequency = lfoRate;
    _lfo.setParameters(lfoPrams);
}