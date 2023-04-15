/*
  ==============================================================================

    ClassATubePre.h
    Created: 7 Sep 2020 3:22:03pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include "Parameters.h"
#include "IAudioProcessor.h"
#include "TriodeClassA.h"
#include "TwoBandShelvingFilter.h"

class ClassATubePre : IAudioProcessor
{
public:
    // Inherited via IAudioProcessor
    virtual bool reset(double sampleRate) override;
    virtual double process(double xn) override;
    virtual bool canProcessAudioFrame() override;

    ClassATubePreParameters getParameters();
    void setParameters(ClassATubePreParameters newParameters);

private:
    ClassATubePreParameters parameters;
    TriodeClassA triodes[NUM_TUBES];
    TwoBandShelvingFilter shelvingFilter;
    double inputLevel = 1.0;
    double outputLevel = 1.0;
};