#include <dsp/AudioProcessorNode.h>

AudioProcessorNode::~AudioProcessorNode()
{
    
}


void AudioProcessorNode::process(double xn)
{
    _yn = xn;
    _finishedProcessing = true;
}

void AudioProcessorNode::process()
{
    _xn = 0.0;

    for (auto input : _inputs)
    {
        _xn += input->getOutputSample();
    }

    for (auto feedbackInput : _feedbackConnections)
    {
        _xn += feedbackInput->getOutputSample();
    }

    if (_processor != nullptr)
    {
        _yn = _processor->process(_xn);
        _finishedProcessing = true;
    }
    else
    {
        _yn = _xn;
    }
}

void AudioProcessorNode::reset()
{
    _inputs.clear();
    _outputs.clear();
}

void AudioProcessorNode::connectInput(AudioProcessorNode* connection)
{
    if (!_inputs.contains(connection))
    {
        _inputs.add(connection);
    }
}

void AudioProcessorNode::connectFeedbackInput(AudioProcessorNode* connection)
{
    if (!_feedbackConnections.contains(connection))
    {
        _feedbackConnections.add(connection);
    }  
}

void AudioProcessorNode::connectOutput(AudioProcessorNode* connection)
{
    if (!_outputs.contains(connection))
    {
        _outputs.add(connection);
    }
}

void AudioProcessorNode::disconnectInput(AudioProcessorNode* connection)
{
    int index = _inputs.indexOf(connection);
    int feedbackIndex = _feedbackConnections.indexOf(connection);

    if (index >= 0)
        _inputs.remove(index);

    if (feedbackIndex >= 0)
        _feedbackConnections.remove(feedbackIndex);
}

void AudioProcessorNode::disconnectOutput(AudioProcessorNode* connection)
{
    int index = _outputs.indexOf(connection);

    if (index >= 0)
        _outputs.remove(index);
}

void AudioProcessorNode::disconnectAll()
{
    for(AudioProcessorNode* input : _inputs)
    {
        disconnectInput(input);
    }

    for(AudioProcessorNode* output : _outputs)
    {
        disconnectInput(output);
    }

     for(AudioProcessorNode* feedback : _feedbackConnections)
    {
        disconnectInput(feedback);
    }
}

bool AudioProcessorNode::isReady()
{
    bool ready = true;
    for (auto input : _inputs)
    {
        if (_feedbackConnections.contains(input))
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
    _processor = processor;
}

IAudioProcessor* AudioProcessorNode::getProcessor()
{
    return _processor.get();
}

double AudioProcessorNode::getInputSample()
{
	return _xn;
}

double AudioProcessorNode::getOutputSample()
{
    return _yn;
}

void AudioProcessorNode::setOutputSample(double sample)
{
    _yn = sample;
    _finishedProcessing = true;
}

Array<AudioProcessorNode*> AudioProcessorNode::getInputConnections()
{
    return _inputs;
}

Array<AudioProcessorNode*> AudioProcessorNode::getOutputConnections()
{
    return _outputs;
}

Array<AudioProcessorNode*> AudioProcessorNode::getReadyOutputConnections()
{
    Array<AudioProcessorNode*> readyBlocks;

    for (auto output : _outputs)
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
    return _finishedProcessing;
}

void AudioProcessorNode::setFinishedProcessing(bool hasFinished)
{
    _finishedProcessing = hasFinished;
}

std::string AudioProcessorNode::getIdAsString()
{
    return _id.toString().toStdString();
}
