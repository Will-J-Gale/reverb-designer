#pragma once
#include <memory>

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
    std::unique_ptr<double[]> buffer;
    unsigned int bufferLength = 0;
    unsigned int writeIndex = 0;
    bool interpolate = false;
};