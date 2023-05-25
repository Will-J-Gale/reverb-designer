#pragma once
#include <dsp/IAudioSignalGenerator.h>
#include <utils/Math.h>
#include <utils/Constants.h>

class LFO : public IAudioSignalGenerator
{
public:
    // Inherited via IAudioSignalGenerator
    virtual void reset(double sampleRate) override;
    virtual void setGeneratorFrequency(double frequency) override;
    virtual void setGeneratorWaveform(GeneratorWaveform waveform) override;
    virtual SignalGenData renderAudioOutput() override;

    OscillatorParameters getParameters();
    bool setParameters(OscillatorParameters newParameters);

    void setSampleRate(double sampleRate);
private:
    bool checkAndWrapModulo(double& moduloCounter, double phaseInc);
    bool advanceAndcheckAndWrapModulo(double& moduloCounter, double phaseInc);
    void advanceModulo(double& moduloCounter, double phaseInc);
    double parabolicSine(double angle);

    OscillatorParameters _lfoParameters;
    double _modCounter = 0.0;
    double modCounterQP = 0.0;
    double _phaseInc = 0.0;
    double _sampleRate = DEFAULT_SAMPLERATE;

    const double _B = 4.0 / Math::PI;
    const double _C = -4.0 / (Math::PI * Math::PI);
    const double _P = 0.225;
};