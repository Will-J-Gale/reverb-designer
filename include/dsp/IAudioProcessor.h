/*
  ==============================================================================

    IAudioProcessor.h
    Created: 22 Aug 2020 1:12:35pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include <cstdint>
#include <memory>
#include <dsp/Parameters.h>
#include <stdexcept>
#include <parameters/AudioParameters.h>

#define IAudioProcessorPtr std::shared_ptr<IAudioProcessor>

class IAudioProcessor
{
public:
    virtual bool reset(double sampleRate) = 0;
    virtual double process(double xn) = 0;
    virtual bool canProcessAudioFrame() = 0;
    virtual AudioParametersPtr getParameters() = 0;

	virtual void setSampleRate(double sampleRate) {};
	virtual void enableAuxInput(bool enableAuxInput) {};
    virtual void onParametersChanged() {};
	virtual double processAuxInputAudioSample(double xn)
	{
		return xn;
	}
	virtual bool processAudioFrame(const float* inputFrame,
		float* outputFrame,
		uint32_t inputChannels,
		uint32_t outputChannels)
	{
		return false;
	}
};