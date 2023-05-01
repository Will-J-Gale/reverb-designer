#include <dsp/Delay.h>

Delay::Delay()
{
    parameters->addOnChangeCallback(std::bind(&Delay::onParametersChanged, this));
    onParametersChanged();
}

bool Delay::reset(double sampleRate)
{
    this->sampleRate = sampleRate;

    buffer.flushBuffer();
    double maxDelayTimeInSeconds = parameters->getParameterByNameAsType<DoubleParameter>("DelayTimeMs")->getMax() / 1000.0;
    bufferLength = (sampleRate * maxDelayTimeInSeconds) + 1;

    buffer.createCircularBuffer(bufferLength);
}

double Delay::process(double xn)
{
    if (delayTimeInSamples == 0)
    {
        return xn;
    }

    double yn = buffer.read(delayTimeInSamples);

    buffer.write(xn);

    return yn;
}
AudioParametersPtr Delay::getParameters()
{
    return parameters;
}

bool Delay::canProcessAudioFrame()
{
    return false;
}

double Delay::readDelay()
{
    return buffer.read(delayTimeInSamples);
}

double Delay::readDelayAtTimeInMs(double delayInMs)
{
    double delayInSamples = sampleRate * (delayInMs / 1000.0);

    return buffer.read(delayInSamples);
}

double Delay::readDelayAtPercentage(double delayPercentage)
{
    double delayInSamples = delayTimeInSamples * delayPercentage;

    return buffer.read(delayInSamples);
}

void Delay::write(double xn)
{
    buffer.write(xn);
}

void Delay::onParametersChanged()
{
    auto delayTimeInMs = parameters->getParameterValueByName<double>("DelayTimeMs");
    delayTimeInSamples = sampleRate * (delayTimeInMs / 1000.0);
}
