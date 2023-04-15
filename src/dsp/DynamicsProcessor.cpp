/*
  ==============================================================================

    DynamicsProcessor.cpp
    Created: 6 Sep 2020 5:56:25pm
    Author:  Will

  ==============================================================================
*/

#include <dsp/DynamicsProcessor.h>
#include <cmath>

bool DynamicsProcessor::reset(double sampleRate)
{
    sideChainInputSample = 0.0;

    detector.reset(sampleRate);

    AudioDetectorParameters detectorParams = detector.getParameters();
    detectorParams.clampToUnityMax = false;
    detectorParams.detectDb = true;
    detector.setParameters(detectorParams);

    return true;
}

double DynamicsProcessor::process(double xn)
{
    double envelope = 0.0;

    if (parameters.enableSideChain)
    {
        envelope = detector.process(sideChainInputSample);
    }
    else
    {
        envelope = detector.process(xn);
    }

    double gr = computeGain(envelope);
    double makeUpGain = pow(10.0, parameters.outputGainDb / 20.0);

    return xn * gr * makeUpGain;
}

bool DynamicsProcessor::canProcessAudioFrame()
{
    return false;
}

DynamicsProcessorParameters DynamicsProcessor::getParameters()
{
    return parameters;
}

void DynamicsProcessor::setParameters(DynamicsProcessorParameters newParameters)
{
    parameters = newParameters;

    AudioDetectorParameters detectorParams = detector.getParameters();
    detectorParams.attackTimeInMs = parameters.attackTimeInMs;
    detectorParams.releaseTimeInMs = parameters.releaseTimeInMs;
    detector.setParameters(detectorParams);
}

double DynamicsProcessor::computeGain(double envelope)
{
    double output = 0.0;
    double threshold = parameters.thresholdDb;
    double ratio = parameters.ratio;
    bool softKnee = parameters.softKnee;
    bool hardLimit = parameters.hardLimit;
    double kneeWidth = parameters.kneeWidthDb;

    if (parameters.calculation == DynamicsProcessorType::Compressor)
    {
        if(!softKnee)
        {
            if (envelope > threshold)
            {
                //Limiter
                if (hardLimit)
                {
                    output = threshold;
                }
                else //Compressor
                {
                    output = output = threshold + ((envelope - threshold) / ratio);
                }
            }
            else
            {
                output = envelope;
            }
        }
        else
        {
            //Left of knee
            if (2.0 * (envelope - threshold) < -kneeWidth)
            {
                output = envelope;
            }
            else if (2.0* (fabs(envelope - threshold) <= kneeWidth)) //Inside knee
            {
                if (hardLimit)
                {
                    output = envelope + (pow((envelope - threshold + (kneeWidth / 2.0)), 2.0)) / (2.0 * kneeWidth);
                }
                else
                {
                    output = envelope + ((1 / (ratio - 1) * pow((envelope - threshold + (kneeWidth / 2.0)), 2.0)) / (2.0 * kneeWidth));
                }
            }
            else if (2.0 * (envelope - threshold) > kneeWidth) //Right of knee (compression zone)
            {
                if (hardLimit)
                {
                    output = threshold;
                }
                else //Compressor
                {
                    output = output = threshold + ((envelope - threshold) / ratio);
                }
            }
        }
    }
    else
    {
        if (!softKnee || hardLimit)
        {
            if (envelope > threshold)
            {
                output = envelope;
            }
            else
            {
                //Gate
                if (hardLimit)
                {
                    output = -1.0e34;
                }
                else //Downward expander
                {
                    output = threshold + (ratio * (envelope - threshold));
                }
            }
        }
        else
        {
            if (2.0 * (envelope - threshold) > kneeWidth)
            {
                output = envelope;
            }
            else if (2.0 * fabs(envelope - threshold) > -kneeWidth)
            {
                output = envelope - ((1 / ratio) * pow(((envelope - threshold - (kneeWidth / 2))), 2.0) / (2 * kneeWidth));
            }
            else if (2.0 * (envelope - threshold) <= -kneeWidth)
            {
                output = threshold + (ratio * (envelope - threshold));
            }
        }
    }

    parameters.gainReductionDb = output - envelope;
    parameters.gainReduction = pow(10.0, parameters.gainReductionDb / 20.0);
    return parameters.gainReduction;
}
