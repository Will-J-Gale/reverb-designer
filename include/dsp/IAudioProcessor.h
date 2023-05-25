
#pragma once
#include <cstdint>
#include <dsp/Parameters.h>
#include <stdexcept>
#include <parameters/AudioParameters.h>

#define IAudioProcessorPtr std::shared_ptr<IAudioProcessor>

class IAudioProcessor
{
public:
    virtual void reset(double sampleRate) = 0;
    virtual double process(double xn) = 0;
    virtual bool canProcessAudioFrame() = 0;

	virtual void setSampleRate(double sampleRate) {};
	virtual void enableAuxInput(bool enableAuxInput) {};
    virtual void onParametersChanged() {};
    virtual AudioParametersPtr getParameters()
    {
        jassert(_parameters != nullptr);
        return _parameters;
    };
    virtual void setParameters(AudioParametersPtr newParameters)
    {
        _parameters = newParameters;
    };
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

protected:
    AudioParametersPtr _parameters = nullptr;
};