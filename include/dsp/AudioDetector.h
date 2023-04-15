/*
  ==============================================================================

    AudioDetector.h
    Created: 30 Aug 2020 12:43:05pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include <dsp/IAudioProcessor.h>
#include <dsp/Parameters.h>

class AudioDetector : IAudioProcessor
{
public:
    virtual bool reset(double sampleRate) override;
    virtual double process(double xn) override;
    virtual bool canProcessAudioFrame() override;

    void setParameters(AudioDetectorParameters newParameters);
    AudioDetectorParameters getParameters();

    void setSampleRate(double newSampleRate);
    
private:
    AudioDetectorParameters parameters;
    double attackCoeff;
    double releaseCoeff;
    double lastEnvelope;
    double sampleRate;

    void setAttackTime(double newAttackTimeInMs);
    void setReleaseTime(double newReleaseTimeInMs);
};