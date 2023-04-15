/*
  ==============================================================================

    DynamicsProcessor.h
    Created: 6 Sep 2020 5:56:25pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include <dsp/Parameters.h>
#include <dsp/IAudioProcessor.h>
#include <dsp/AudioDetector.h>

class DynamicsProcessor : IAudioProcessor
{
public:
    // Inherited via IAudioProcessor
    virtual bool reset(double sampleRate) override;
    virtual double process(double xn) override;
    virtual bool canProcessAudioFrame() override;

    DynamicsProcessorParameters getParameters();
    void setParameters(DynamicsProcessorParameters newParameters);

private:
    double computeGain(double envelope);

    DynamicsProcessorParameters parameters;
    AudioDetector detector;
    double sideChainInputSample;
};