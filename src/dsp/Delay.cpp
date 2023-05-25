#include <dsp/Delay.h>

Delay::Delay()
{
    _parameters = MAKE_PARAMETERS({
        std::make_shared<DoubleParameter>("TimeMs", 0.0, 0.0, 5000.0)
    });

    _parameters->addOnChangeCallback(std::bind(&Delay::onParametersChanged, this));
    onParametersChanged();
}

void Delay::reset(double sampleRate)
{
    _sampleRate = sampleRate;

    _buffer.flushBuffer();
    double maxDelayTimeInSeconds = _parameters->getParameterByNameAsType<DoubleParameter>("TimeMs")->getMax() / 1000.0;
    _bufferLength = (sampleRate * maxDelayTimeInSeconds) + 1;

    _buffer.createCircularBuffer(_bufferLength);
}

double Delay::process(double xn)
{
    if (_delayTimeInSamples == 0)
    {
        return xn;
    }

    double yn = _buffer.read(_delayTimeInSamples);

    _buffer.write(xn);

    return yn;
}

bool Delay::canProcessAudioFrame()
{
    return false;
}

double Delay::readDelay()
{
    return _buffer.read(_delayTimeInSamples);
}

double Delay::readDelayAtTimeInMs(double delayInMs)
{
    double delayInSamples = _sampleRate * (delayInMs / 1000.0);

    return _buffer.read(delayInSamples);
}

double Delay::readDelayAtPercentage(double delayPercentage)
{
    double delayInSamples = _delayTimeInSamples * delayPercentage;

    return _buffer.read(delayInSamples);
}

void Delay::write(double xn)
{
    _buffer.write(xn);
}

void Delay::onParametersChanged()
{
    auto delayTimeInMs = _parameters->getParameterValueByName<double>("TimeMs");
    _delayTimeInSamples = _sampleRate * (delayTimeInMs / 1000.0);
}
