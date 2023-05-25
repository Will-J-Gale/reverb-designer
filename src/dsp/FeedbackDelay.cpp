#include <dsp/FeedbackDelay.h>

FeedbackDelay::FeedbackDelay()
{
    _parameters = MAKE_PARAMETERS({
        std::make_shared<DoubleParameter>("Mix", DEFAULT_MIX, 0.0, 1.0),
        std::make_shared<DoubleParameter>("DelayInSeconds", DEFAULT_DELAY_TIME, 0.0, 5.0),
        std::make_shared<DoubleParameter>("Feedback", DEFAULT_FEEDBACK, 0.0, 1.0),
    });

    _parameters->addOnChangeCallback(std::bind(&FeedbackDelay::onParametersChanged, this));
    onParametersChanged();
}

void FeedbackDelay::reset(double sampleRate)
{
    if (_sampleRate == sampleRate)
    {
        _buffer.flushBuffer();
        return;
    }

    createDelayBuffers(sampleRate, _bufferLengthInSeconds);
}

double FeedbackDelay::process(double xn)
{
    double delay = _buffer.read(_delayInSamples);
    double mix = _parameters->getParameterValueByName<double>("Mix");
    double feedbackAmount = _parameters->getParameterValueByName<double>("Feedback");
    double yn = (delay * mix) + (xn * (1.0 - mix));

    double feedbackSample = delay * feedbackAmount;
    _buffer.write(xn + feedbackSample);

    return yn;
}

bool FeedbackDelay::canProcessAudioFrame()
{
	return false;
}

void FeedbackDelay::createDelayBuffers(double sampleRate, float bufferLengthInSeconds)
{
    _bufferLengthInSeconds = bufferLengthInSeconds;
    _sampleRate = sampleRate;
    _bufferLength = sampleRate * bufferLengthInSeconds;
    _buffer.createCircularBuffer(_bufferLength);
}

void FeedbackDelay::onParametersChanged()
{
    double delayInSeconds = _parameters->getParameterValueByName<double>("DelayInSeconds");
    _delayInSamples = _sampleRate * delayInSeconds;
}