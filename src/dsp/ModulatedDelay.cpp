#include <dsp/ModulatedDelay.h>
#include <utils/Math.h>

ModulatedDelay::ModulatedDelay()
{
    parameters->addOnChangeCallback(std::bind(&ModulatedDelay::onParametersChanged, this));
    onParametersChanged();
}
bool ModulatedDelay::reset(double sampleRate)
{
    delay.reset(sampleRate);
    delay.createDelayBuffers(sampleRate, 1);
    delayParameters = delay.getParameters();
    lfo.reset(sampleRate);

    return true;
}

double ModulatedDelay::process(double xn)
{
    SignalGenData lfoOutput = lfo.renderAudioOutput();
    double minDelayMs = 0.0;
    double maxDelayMs = 0.0;
    ModulatedDelayAlgorithm algorithm = parameters->getParameterValueByName<ModulatedDelayAlgorithm>("Algorithm");
    double lfoDepth = parameters->getParameterValueByName<double>("LFO Depth");
    double lfoRate = parameters->getParameterValueByName<double>("LFO Rate");
    double mix = 0.0;
    double feedback = 0.0;

    if (algorithm == ModulatedDelayAlgorithm::kFlanger)
    {
        minDelayMs = 0.1;
        maxDelayMs = 7.0;
        mix = 0.5;
    }
    else if (algorithm == ModulatedDelayAlgorithm::kChorus)
    {
        minDelayMs = 10.0;
        maxDelayMs = 30.0;
        mix = 0.4;
        feedback = 0.0;
    }
    else if (algorithm == ModulatedDelayAlgorithm::kVibrato)
    {
        minDelayMs = 0.01;
        maxDelayMs = 7.0;
        mix = 1.0;
        feedback = 0.0;
    }
    
    delayParameters->setParameterValueByName<DoubleParameter, double>("Feedback", feedback);

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

    delayParameters->setParameterValueByName<DoubleParameter, double>("DelayInSeconds", delayInSeconds);

    return delay.process(xn);
}

bool ModulatedDelay::canProcessAudioFrame()
{
    return false;
}

AudioParametersPtr ModulatedDelay::getParameters()
{
    return parameters;
}

void ModulatedDelay::onParametersChanged()
{
    OscillatorParameters lfoParams = lfo.getParameters();
    lfoParams.frequency = parameters->getParameterValueByName<double>("LFO Rate");
    
    ModulatedDelayAlgorithm algorithm = parameters->getParameterValueByName<ModulatedDelayAlgorithm>("Algorithm");

    if(algorithm == ModulatedDelayAlgorithm::kVibrato)
        lfoParams.waveForm = GeneratorWaveform::kSin;
    else
        lfoParams.waveForm = GeneratorWaveform::kTriangle;

    double feedback = parameters->getParameterValueByName<double>("Feedback");
    delayParameters->setParameterValueByName<DoubleParameter, double>("Feedback", feedback);
    lfo.setParameters(lfoParams);
}

