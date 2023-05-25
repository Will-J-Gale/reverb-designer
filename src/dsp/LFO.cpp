#include <cmath>
#include <dsp/LFO.h>

void LFO::reset(double sampleRate)
{
    _sampleRate = sampleRate;
    _phaseInc = _lfoParameters.frequency / sampleRate;
    _modCounter = 0.0;
    modCounterQP = 0.25;
}

void LFO::setGeneratorFrequency(double frequency)
{
}

void LFO::setGeneratorWaveform(GeneratorWaveform waveform)
{
}

SignalGenData LFO::renderAudioOutput()
{
    checkAndWrapModulo(_modCounter, _phaseInc);

    modCounterQP = _modCounter;
    advanceAndcheckAndWrapModulo(modCounterQP, 0.25);

    SignalGenData output;

    if (_lfoParameters.waveForm == GeneratorWaveform::kSin)
    {
        double angle = (_modCounter * 2.0 * Math::PI) - Math::PI;

        output.normalOutput = parabolicSine(-angle);

        angle = (modCounterQP * 2.0 * Math::PI) - Math::PI;

        output.quadPhaseOutput_pos = parabolicSine(-angle);
    }
    else if (_lfoParameters.waveForm == GeneratorWaveform::kTriangle)
    {
        double bipolar = Math::unipolarToBipolar(_modCounter);
        output.normalOutput = 2.0 * fabs(bipolar) - 1;

        bipolar = Math::unipolarToBipolar(modCounterQP);
        output.quadPhaseOutput_pos = 2.0 * fabs(bipolar) - 1;
    }
    else if (_lfoParameters.waveForm == GeneratorWaveform::kSaw)
    {
        output.normalOutput = Math::unipolarToBipolar(_modCounter);
        output.quadPhaseOutput_pos = Math::unipolarToBipolar(modCounterQP);
    }

    output.invertedOutput = -output.normalOutput;
    output.quadPhaseOutput_neg = -output.quadPhaseOutput_pos;

    advanceModulo(_modCounter, _phaseInc);

    return output;
}

OscillatorParameters LFO::getParameters()
{
    return _lfoParameters;
}

bool LFO::setParameters(OscillatorParameters newParameters)
{
    _lfoParameters = newParameters;
    _phaseInc = _lfoParameters.frequency / _sampleRate;
    return true;
}

void LFO::setSampleRate(double sampleRate)
{
    _sampleRate = sampleRate;
}

bool LFO::checkAndWrapModulo(double& moduloCounter, double phaseInc)
{
    if (phaseInc > 0 && moduloCounter >= 1.0)
    {
        moduloCounter -= 1.0;
        return true;
    }

    // --- for negative frequencies
    if (phaseInc < 0 && moduloCounter <= 0.0)
    {
        moduloCounter += 1.0;
        return true;
    }

    return false;
}

bool LFO::advanceAndcheckAndWrapModulo(double& moduloCounter, double phaseInc)
{
    // --- advance counter
    moduloCounter += phaseInc;

    // --- for positive frequencies
    if (phaseInc > 0 && moduloCounter >= 1.0)
    {
        moduloCounter -= 1.0;
        return true;
    }

    // --- for negative frequencies
    if (phaseInc < 0 && moduloCounter <= 0.0)
    {
        moduloCounter += 1.0;
        return true;
    }

    return false;
}

void LFO::advanceModulo(double& moduloCounter, double phaseInc)
{
    moduloCounter += phaseInc;
}

double LFO::parabolicSine(double angle)
{
    double y = _B * angle + _C * angle * fabs(angle);
    y = _P * (y * fabs(y) - y) + y;
    return y;
}
