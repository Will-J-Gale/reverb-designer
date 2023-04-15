/*
  ==============================================================================

    ClassATubePre.cpp
    Created: 7 Sep 2020 3:22:03pm
    Author:  Will

  ==============================================================================
*/

#include <dsp/ClassATubePre.h>
#include <cmath>

bool ClassATubePre::reset(double sampleRate)
{
    TriodeClassAParameters tubeParams = triodes[0].getParameters();
    tubeParams.invertOutput = true;
    tubeParams.enableHPF = true;
    tubeParams.outputGain = 1.0;
    tubeParams.saturation = 1.0;
    tubeParams.asymmetry = 1.0;
    tubeParams.enableLSF = true;
    tubeParams.lsfFreq = 88.0;
    tubeParams.lsfGain = -12;
    tubeParams.waveshaper = DistortionModel::FuzzAsym;

    for (size_t i = 0; i < NUM_TUBES; i++)
    {
        triodes[i].reset(sampleRate);
        triodes[i].setParameters(tubeParams);
    }

    shelvingFilter.reset(sampleRate);

    return true;
}

double ClassATubePre::process(double xn)
{
    double output = xn * inputLevel;

    for (size_t i = 0; i < NUM_TUBES - 1; i++)
    {
        output = triodes[i].process(output);
    }

    output = shelvingFilter.process(output);
    output = triodes[NUM_TUBES - 1].process(output);

    return output * outputLevel;
}

bool ClassATubePre::canProcessAudioFrame()
{
    return false;
}

ClassATubePreParameters ClassATubePre::getParameters()
{
    return parameters;
}

void ClassATubePre::setParameters(ClassATubePreParameters newParameters)
{
    parameters = newParameters;

    inputLevel = pow(10.0, parameters.inputLevelDb / 20.0);
    outputLevel = pow(10.0, parameters.outputLevelDb / 20.0);

    TwoBandShelvingFilterParameters shelfParams = shelvingFilter.getParameters();
    shelfParams.highSelfFreq = parameters.highShelfFc;
    shelfParams.highShelfGainInDb = parameters.highShelfGain;
    shelfParams.lowSelfFreq = parameters.lowShelfFc;
    shelfParams.lowShelfGainInDb = parameters.lowShelfGain;

    shelvingFilter.setParameters(shelfParams);

    TriodeClassAParameters tubeParams = triodes[0].getParameters();
    tubeParams.saturation = parameters.saturation;

    for (size_t i = 0; i < NUM_TUBES; i++)
    {
        triodes[i].setParameters(tubeParams);
    }
}
