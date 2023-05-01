#pragma once
#include <dsp/IAudioSignalGenerator.h>
#include <utils/Math.h>

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

    OscillatorParameters lfoParameters;
    double modCounter;
    double modCounterQP;
    double phaseInc;
    double sampleRate;

    const double B = 4.0 / Math::PI;
    const double C = -4.0 / (Math::PI * Math::PI);
    const double P = 0.225;
};