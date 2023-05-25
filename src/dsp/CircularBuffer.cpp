#include <dsp/CircularBuffer.h>
#include <utils/Math.h>
#include <cstring>

double CircularBuffer::read(int delayInSamples)
{
    int readIndex = _writeIndex - delayInSamples;

    if (readIndex < 0)
        readIndex += _bufferLength;

    return _buffer[readIndex];
}

double CircularBuffer::read(double delayInFractionalSamples, bool interpolate)
{
    int delayInSamplesInt = (int)delayInFractionalSamples;
    double y1 = read(delayInSamplesInt);

    if (!interpolate) { return y1; }

    double y2 = read(delayInSamplesInt + 1);

    double fraction = delayInFractionalSamples - delayInSamplesInt;

    return Math::linearInterpolation(y1, y2, fraction);
}

void CircularBuffer::createCircularBuffer(size_t bufferLength)
{
    _writeIndex = 0;
    _bufferLength = bufferLength;
    _buffer.reset(new double[_bufferLength]);

    flushBuffer();
}

void CircularBuffer::write(double sample)
{
    _buffer[_writeIndex] = sample;
    _writeIndex = (_writeIndex + 1) % _bufferLength;
}

void CircularBuffer::flushBuffer()
{
    memset(&_buffer[0], 0, _bufferLength * sizeof(double));
}

void CircularBuffer::setInterpolate(bool interpolate)
{
    _interpolate = interpolate;
}
