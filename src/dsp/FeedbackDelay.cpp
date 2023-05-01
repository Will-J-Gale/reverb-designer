#include <dsp/FeedbackDelay.h>

FeedbackDelay::FeedbackDelay()
{
    parameters->addOnChangeCallback(std::bind(&FeedbackDelay::onParametersChanged, this));
    onParametersChanged();
}

bool FeedbackDelay::reset(double sampleRate)
{
    if (this->sampleRate == sampleRate)
    {
        buffer.flushBuffer();
        return true;
    }

    createDelayBuffers(sampleRate, bufferLengthInSeconds);
    return true;
}

double FeedbackDelay::process(double xn)
{
    double delay = buffer.read(delayInSamples);
    double mix = parameters->getParameterValueByName<double>("Mix");
    double feedbackAmount = parameters->getParameterValueByName<double>("Feedback");
    double yn = (delay * mix) + (xn * (1.0 - mix));

    double feedbackSample = delay * feedbackAmount;
    buffer.write(xn + feedbackSample);

    return yn;
}

bool FeedbackDelay::canProcessAudioFrame()
{
	return false;
}

void FeedbackDelay::createDelayBuffers(double sampleRate, float bufferLengthInSeconds)
{
    this->bufferLengthInSeconds = bufferLengthInSeconds;
    this->sampleRate = sampleRate;

    this->bufferLength = sampleRate * bufferLengthInSeconds;
    buffer.createCircularBuffer(bufferLength);
}

AudioParametersPtr FeedbackDelay::getParameters()
{
    return parameters;
}

void FeedbackDelay::onParametersChanged()
{
    double delayInSeconds = parameters->getParameterValueByName<double>("DelayInSeconds");
    delayInSamples = sampleRate * delayInSeconds;
}