#pragma once

class CircularBuffer
{
public:

    double read(int delayInSamples);
    double read(double delayInFractionalSamples, bool interpolate);
    void createCircularBuffer(size_t bufferLength);
    void write(double sample);
    void flushBuffer();
    void setInterpolate(bool interpolate);

private:
    std::unique_ptr<double[]> _buffer;
    unsigned int _bufferLength = 0;
    unsigned int _writeIndex = 0;
    bool _interpolate = false;
};