/*
  ==============================================================================

    AudioProcessingBlock.cpp
    Created: 20 Sep 2020 4:16:08pm
    Author:  Will

  ==============================================================================
*/

#include <dsp/AudioProcessingBlock.h>

AudioProcessingBlock::~AudioProcessingBlock()
{
    /*if (processor != nullptr)
    {
        delete processor;
        processor = nullptr;
    }*/
}


void AudioProcessingBlock::process(double xn)
{
    yn = xn;
    finishedProcessing = true;
}

void AudioProcessingBlock::process()
{
    xn = 0.0;

    for (auto input : inputs)
    {
        xn += input->getOutputSample();
    }

    for (auto feedbackInput : feedbackConnections)
    {
        xn += feedbackInput->getOutputSample();
    }

    if (processor != nullptr)
    {
        yn = processor->process(xn);
        finishedProcessing = true;
    }
    else
    {
        yn = xn;
    }
}

void AudioProcessingBlock::reset()
{
    inputs.clear();
    outputs.clear();
}

void AudioProcessingBlock::connectInput(AudioProcessingBlock* connection)
{
    if (!inputs.contains(connection))
    {
        inputs.add(connection);
    }
}

void AudioProcessingBlock::connectFeedbackInput(AudioProcessingBlock* connection)
{
    if (!feedbackConnections.contains(connection))
    {
        feedbackConnections.add(connection);
    }  
}

void AudioProcessingBlock::connectOutput(AudioProcessingBlock* connection)
{
    if (!outputs.contains(connection))
    {
        outputs.add(connection);
    }
}

void AudioProcessingBlock::disconnectInput(AudioProcessingBlock* connection)
{
    int index = inputs.indexOf(connection);
    int feedbackIndex = feedbackConnections.indexOf(connection);

    if (index >= 0)
        inputs.remove(index);

    if (feedbackIndex >= 0)
        feedbackConnections.remove(feedbackIndex);
}

void AudioProcessingBlock::disconnectOutput(AudioProcessingBlock* connection)
{
    int index = outputs.indexOf(connection);

    if (index >= 0)
        outputs.remove(index);
}

bool AudioProcessingBlock::isReady()
{
    bool ready = true;
    for (auto input : inputs)
    {
        if (feedbackConnections.contains(input))
        {
            continue;
        }

        if (!input->hasFinishedProcessing())
        {
            ready = false;
        }
    }

    return ready;
}

void AudioProcessingBlock::setProcessor(IAudioProcessorPtr processor)
{
    this->processor = processor;
}

IAudioProcessor* AudioProcessingBlock::getProcessor()
{
    return processor.get();
}

double AudioProcessingBlock::getInputSample()
{
	return xn;
}

double AudioProcessingBlock::getOutputSample()
{
    return yn;
}

void AudioProcessingBlock::setOutputSample(double sample)
{
    yn = sample;
    finishedProcessing = true;
}

Array<AudioProcessingBlock*> AudioProcessingBlock::getInputConnections()
{
    return inputs;
}

Array<AudioProcessingBlock*> AudioProcessingBlock::getOutputConnections()
{
    return outputs;
}

Array<AudioProcessingBlock*> AudioProcessingBlock::getReadyOutputConnections()
{
    Array<AudioProcessingBlock*> readyBlocks;

    for (auto output : outputs)
    {
        if (output->isReady())
        {
            readyBlocks.add(output);
        }
    }

    return readyBlocks;
}

bool AudioProcessingBlock::hasFinishedProcessing()
{
    return finishedProcessing;
}

void AudioProcessingBlock::setFinishedProcessing(bool hasFinished)
{
    finishedProcessing = hasFinished;
}

std::string AudioProcessingBlock::getIdAsString()
{
    return id.toString().toStdString();
}
