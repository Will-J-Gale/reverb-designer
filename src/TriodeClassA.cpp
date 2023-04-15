/*
  ==============================================================================

    TriodeClassA.cpp
    Created: 7 Sep 2020 2:32:56pm
    Author:  Will

  ==============================================================================
*/

#include "TriodeClassA.h"
#include "WaveshaperFunctions.h"

TriodeClassA::TriodeClassA()
{
    AudioFilterParameters hpfParams = hpf.getParameters();
    hpfParams.filterType = FilterType::HPF1;
    hpfParams.fc = parameters.hpfFreq;
    hpf.setParameters(hpfParams);

    AudioFilterParameters lsfParams = lsf.getParameters();
    lsfParams.filterType = FilterType::LSF;
    lsfParams.fc = parameters.lsfFreq;
    lsfParams.gain_dB = parameters.lsfGain;
    lsf.setParameters(lsfParams);
}

bool TriodeClassA::reset(double sampleRate)
{
    return false;
}

double TriodeClassA::process(double xn)
{
    double output = 0.0;
    double saturation = parameters.saturation;
    double asymmetry = parameters.asymmetry;

    if (parameters.waveshaper == DistortionModel::SoftClip)
    {
        output = softClipWaveshaper(xn, saturation);
    }
    else if (parameters.waveshaper == DistortionModel::ArcTan)
    {
        output = atanWaveshaper(xn, saturation);
    }
    else if (parameters.waveshaper == DistortionModel::FuzzAsym)
    {
        output = fuzzExp1Waveshaper(xn, saturation, asymmetry);
    }

    if (parameters.invertOutput)
    {
        output *= -1;
    }

    if (parameters.enableHPF)
    {
        output = hpf.process(output);
    }

    if (parameters.enableLSF)
    {
        output = lsf.process(output);
    }

    return output * parameters.outputGain;
}

bool TriodeClassA::canProcessAudioFrame()
{
    return false;
}

TriodeClassAParameters TriodeClassA::getParameters()
{
    return parameters;
}

void TriodeClassA::setParameters(TriodeClassAParameters newParameters)
{
    parameters = newParameters;

    AudioFilterParameters hpfParams = hpf.getParameters();
    hpfParams.fc = parameters.hpfFreq;
    hpf.setParameters(hpfParams);

    AudioFilterParameters lsfParams = lsf.getParameters();
    lsfParams.fc = parameters.lsfFreq;
    lsfParams.gain_dB = parameters.lsfGain;
    lsf.setParameters(lsfParams);
}
