#include <dsp/AudioProcessorNode.h>

AudioProcessorNode::~AudioProcessorNode()
{
    
}


void AudioProcessorNode::process(double xn)
{
    yn = xn;
    finishedProcessing = true;
}

void AudioProcessorNode::process()
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

void AudioProcessorNode::reset()
{
    inputs.clear();
    outputs.clear();
}

void AudioProcessorNode::connectInput(AudioProcessorNode* connection)
{
    if (!inputs.contains(connection))
    {
        inputs.add(connection);
    }
}

void AudioProcessorNode::connectFeedbackInput(AudioProcessorNode* connection)
{
    if (!feedbackConnections.contains(connection))
    {
        feedbackConnections.add(connection);
    }  
}

void AudioProcessorNode::connectOutput(AudioProcessorNode* connection)
{
    if (!outputs.contains(connection))
    {
        outputs.add(connection);
    }
}

void AudioProcessorNode::disconnectInput(AudioProcessorNode* connection)
{
    int index = inputs.indexOf(connection);
    int feedbackIndex = feedbackConnections.indexOf(connection);

    if (index >= 0)
        inputs.remove(index);

    if (feedbackIndex >= 0)
        feedbackConnections.remove(feedbackIndex);
}

void AudioProcessorNode::disconnectOutput(AudioProcessorNode* connection)
{
    int index = outputs.indexOf(connection);

    if (index >= 0)
        outputs.remove(index);
}

bool AudioProcessorNode::isReady()
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

void AudioProcessorNode::setProcessor(IAudioProcessorPtr processor)
{
    this->processor = processor;
}

IAudioProcessor* AudioProcessorNode::getProcessor()
{
    return processor.get();
}

double AudioProcessorNode::getInputSample()
{
	return xn;
}

double AudioProcessorNode::getOutputSample()
{
    return yn;
}

void AudioProcessorNode::setOutputSample(double sample)
{
    yn = sample;
    finishedProcessing = true;
}

Array<AudioProcessorNode*> AudioProcessorNode::getInputConnections()
{
    return inputs;
}

Array<AudioProcessorNode*> AudioProcessorNode::getOutputConnections()
{
    return outputs;
}

Array<AudioProcessorNode*> AudioProcessorNode::getReadyOutputConnections()
{
    Array<AudioProcessorNode*> readyBlocks;

    for (auto output : outputs)
    {
        if (output->isReady())
        {
            readyBlocks.add(output);
        }
    }

    return readyBlocks;
}

bool AudioProcessorNode::hasFinishedProcessing()
{
    return finishedProcessing;
}

void AudioProcessorNode::setFinishedProcessing(bool hasFinished)
{
    finishedProcessing = hasFinished;
}

std::string AudioProcessorNode::getIdAsString()
{
    return id.toString().toStdString();
}
