#include <dsp/ModulatedDelay.h>
#include <utils/Math.h>

ModulatedDelay::ModulatedDelay()
{
    _parameters = MAKE_PARAMETERS({
        std::make_shared<OptionParameter>(
            "Algorithm", 
            std::vector<OptionItem> {
                OptionItem("Flanger", (int)ModulatedDelayAlgorithm::kFlanger),
                OptionItem("Vibrato", (int)ModulatedDelayAlgorithm::kVibrato),
                OptionItem("Chorus", (int)ModulatedDelayAlgorithm::kChorus),
            }, 
            2
        ),
        std::make_shared<DoubleParameter>("LFO Rate", DEFAULT_MOD_DELAY_RATE, 0.0, 20.0),
        std::make_shared<DoubleParameter>("LFO Depth", DEFAULT_MOD_DELAY_DEPTH, 0.0, 1.0),
        std::make_shared<DoubleParameter>("Feedback", DEFAULT_MOD_DELAY_FEEDBACK, 0.0, 1.0),
    });

    _delayParameters = _delay.getParameters();
    _parameters->addOnChangeCallback(std::bind(&ModulatedDelay::onParametersChanged, this));
    onParametersChanged();
}
void ModulatedDelay::reset(double sampleRate)
{
    _delay.reset(sampleRate);
    _delay.createDelayBuffers(sampleRate, 1);
    _delayParameters = _delay.getParameters();
    _lfo.reset(sampleRate);
}

double ModulatedDelay::process(double xn)
{
    SignalGenData lfoOutput = _lfo.renderAudioOutput();
    double minDelayMs = 0.0;
    double maxDelayMs = 0.0;
    ModulatedDelayAlgorithm algorithm = _parameters->getParameterValueByName<ModulatedDelayAlgorithm>("Algorithm");
    double lfoDepth = _parameters->getParameterValueByName<double>("LFO Depth");
    double feedback = 0.0;

    if (algorithm == ModulatedDelayAlgorithm::kFlanger)
    {
        minDelayMs = 0.1;
        maxDelayMs = 7.0;
    }
    else if (algorithm == ModulatedDelayAlgorithm::kChorus)
    {
        minDelayMs = 10.0;
        maxDelayMs = 30.0;
        feedback = 0.0;
    }
    else if (algorithm == ModulatedDelayAlgorithm::kVibrato)
    {
        minDelayMs = 0.01;
        maxDelayMs = 7.0;
        feedback = 0.0;
    }
    
    _delayParameters->setParameterValueByName<double>("Feedback", feedback);

    double depth = lfoDepth;
    double modulationMin = minDelayMs;
    double modulationMax = minDelayMs + maxDelayMs;
    double modulationValue = lfoOutput.normalOutput * depth;
    double delayInSeconds = 0.0;

    if (algorithm == ModulatedDelayAlgorithm::kFlanger)
    {
        modulationValue = Math::bipolarToUnipolar(modulationValue);
        delayInSeconds = Math::uniPolarScale(modulationValue, modulationMin, modulationMax) / 1000;
    }
    else
    {
        delayInSeconds = Math::bipolarScale(modulationValue, modulationMin, modulationMax) / 1000;
    }

    _delayParameters->setParameterValueByName<double>("DelayInSeconds", delayInSeconds);

    return _delay.process(xn);
}

bool ModulatedDelay::canProcessAudioFrame()
{
    return false;
}

void ModulatedDelay::onParametersChanged()
{
    OscillatorParameters lfoParams = _lfo.getParameters();
    lfoParams.frequency = _parameters->getParameterValueByName<double>("LFO Rate");
    
    ModulatedDelayAlgorithm algorithm = _parameters->getParameterValueByName<ModulatedDelayAlgorithm>("Algorithm");

    if(algorithm == ModulatedDelayAlgorithm::kVibrato)
        lfoParams.waveForm = GeneratorWaveform::kSin;
    else
        lfoParams.waveForm = GeneratorWaveform::kTriangle;

    double feedback = _parameters->getParameterValueByName<double>("Feedback");
    _delayParameters->setParameterValueByName<double>("Feedback", feedback);
    _lfo.setParameters(lfoParams);
}

