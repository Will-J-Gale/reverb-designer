/*
  ==============================================================================

    ModulatedDelay.cpp
    Created: 31 Aug 2020 12:54:44pm
    Author:  Will

  ==============================================================================
*/

#include "ModulatedDelay.h"
#include "Utilities.h"

bool ModulatedDelay::reset(double sampleRate)
{
    delay.reset(sampleRate);
    delay.createDelayBuffers(sampleRate, 1);
    lfo.reset(sampleRate);

    return true;
}

double ModulatedDelay::process(double xn)
{
    SignalGenData lfoOutput = lfo.renderAudioOutput();

    AudioDelayParameters delayParams = delay.getParameters();
    double minDelayMs = 0.0;
    double maxDelayMs = 0.0;

    if (parameters.algorithm == ModulatedDelayAlgorithm::kFlanger)
    {
        minDelayMs = 0.1;
        maxDelayMs = 7.0;
        delayParams.mix = 0.5;
    }

    if (parameters.algorithm == ModulatedDelayAlgorithm::kChorus)
    {
        minDelayMs = 10.0;
        maxDelayMs = 30.0;
        delayParams.mix = 0.4;
        delayParams.feedback = 0.0;
    }
    
    if (parameters.algorithm == ModulatedDelayAlgorithm::kVibrato)
    {
        minDelayMs = 0.01;
        maxDelayMs = 7.0;
        delayParams.mix = 1.0;
        delayParams.feedback = 0.0;
    }

    double depth = parameters.lfoDepth;
    double modulationMin = minDelayMs;
    double modulationMax = minDelayMs + maxDelayMs;
    double modulationValue = lfoOutput.normalOutput * depth;

    if (parameters.algorithm == ModulatedDelayAlgorithm::kFlanger)
    {
        modulationValue = bipolarToUnipolar(modulationValue);
        delayParams.delayInSeconds = uniPolarScale(modulationValue, modulationMin, modulationMax) / 1000;
    }
    else
    {
        delayParams.delayInSeconds = bipolarScale(modulationValue, modulationMin, modulationMax) / 1000;
    }

    delay.setParameters(delayParams);

    return delay.process(xn);
}

bool ModulatedDelay::canProcessAudioFrame()
{
    return false;
}

void ModulatedDelay::setParamters(ModulatedDelayParameters newParameters)
{
    parameters = newParameters;

    OscillatorParameters lfoParams = lfo.getParameters();
    lfoParams.frequency = parameters.lfoRate;

    if (parameters.algorithm == ModulatedDelayAlgorithm::kVibrato)
    {
        lfoParams.waveForm = GeneratorWaveform::kSin;
    }
    else
    {
        lfoParams.waveForm = GeneratorWaveform::kTriangle;
    }

    AudioDelayParameters delayParams = delay.getParameters();
    delayParams.feedback = parameters.feedback;

    lfo.setParameters(lfoParams);
    delay.setParameters(delayParams);
}

ModulatedDelayParameters ModulatedDelay::getParameters()
{
    return parameters;
}
