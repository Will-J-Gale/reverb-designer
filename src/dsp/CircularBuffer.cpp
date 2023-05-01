#include <dsp/CircularBuffer.h>
#include <utils/Math.h>
#include <cstring>

double CircularBuffer::read(int delayInSamples)
{
    int readIndex = writeIndex - delayInSamples;

    if (readIndex < 0)
    {
        readIndex += bufferLength;
    }

    return buffer[readIndex];
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
    writeIndex = 0;
    this->bufferLength = bufferLength;

    buffer.reset(new double[this->bufferLength]);

    flushBuffer();
}

void CircularBuffer::write(double sample)
{
    buffer[writeIndex] = sample;
    writeIndex = (writeIndex + 1) % bufferLength;
}

void CircularBuffer::flushBuffer()
{
    memset(&buffer[0], 0, bufferLength * sizeof(double));
}

void CircularBuffer::setInterpolate(bool interpolate)
{
    this->interpolate = interpolate;
}
