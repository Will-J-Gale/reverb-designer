/*
  ==============================================================================

    BaseAudioProcessor.cpp
    Created: 10 Sep 2020 6:52:28pm
    Author:  Will

  ==============================================================================
*/

#include <ui/nodes/audioProcessors/AudioProcessorNodeUI.h>
#include <ui/GraphEditor.h>
#include <dsp/Delay.h>


AudioProcessorNodeUI::AudioProcessorNodeUI(String name) : NodeUI(name, NodeUIType::AudioProcessor)
{
    initialize();
}

AudioProcessorNodeUI::AudioProcessorNodeUI(AudioProcessorNodeType type)
{
    this->type = type;
    initialize();
}

AudioProcessorNodeUI::~AudioProcessorNodeUI()
{
}

Array<NodeConnectorUI*> AudioProcessorNodeUI::getAllNodeConnectors()
{
    Array<NodeConnectorUI*> nodes;

    if (input != nullptr)
        nodes.add(input.get());

    if (output != nullptr)
        nodes.add(output.get());

    return nodes;
}

NodeConnectorUI* AudioProcessorNodeUI::getInputNode()
{
    return input.get();
}

NodeConnectorUI* AudioProcessorNodeUI::getOutputNode()
{
	return output.get();
}

void AudioProcessorNodeUI::addInputConnector()
{
    input = std::make_shared<NodeConnectorUI>(this);
    input->setType(NodeType::AudioInput);
    input->setCentrePosition(NODE_SIZE / 2, getHeight() / 2);
    
    addAndMakeVisible(input.get());  
}

void AudioProcessorNodeUI::addOutputConnector()
{
    output = std::make_shared<NodeConnectorUI>(this);
    output->setType(NodeType::AudioOutput);
    output->setCentrePosition(getWidth() - (NODE_SIZE / 2), getHeight() / 2);
    
    addAndMakeVisible(output.get());
}

AudioProcessorNodePtr AudioProcessorNodeUI::getProcessorNode()
{
	return processorNode;
}

void AudioProcessorNodeUI::setProcessorNode(AudioProcessorNodePtr processorNode)
{
    if (processorNode->getType() != type)
    {
        jassert("Cannot pass processing block of different type to UI");
    }

    this->processorNode = processorNode;
    this->setUIParameters();
}

IAudioProcessor* AudioProcessorNodeUI::getAudioProcessor()
{
    return processorNode->getProcessor();
}

void AudioProcessorNodeUI::reverse()
{
    reversed = !reversed;

    auto inputPosition = input->getPosition();
    auto outputPosition = output->getPosition();

    inputPosition.addXY(input->getWidth() / 2, input->getHeight() / 2);
    outputPosition.addXY(output->getWidth() / 2, output->getHeight() / 2);

    input->setCentrePosition(outputPosition);
    output->setCentrePosition(inputPosition);
}

std::string AudioProcessorNodeUI::getIdAsString()
{
    return processorNode->getIdAsString();
}

AudioProcessorNodeType AudioProcessorNodeUI::getType()
{
    return type;
}

bool AudioProcessorNodeUI::isReversed()
{
    return reversed;
}

void AudioProcessorNodeUI::connectInput(NodeConnectorUI* connection)
{
    // if (!inputConnections.contains(connection))
    // {
    //     inputConnections.add(connection);
    //     this->processorNode->connectInput(connection->getProcessorNode().get());
    // }
    
}

void AudioProcessorNodeUI::connectFeedbackInput(NodeConnectorUI* connection)
{
    // if (!feedbackConnections.contains(connection))
    // {
    //     feedbackConnections.add(connection);
    //     this->processorNode->connectFeedbackInput(connection->getProcessorNode().get());
    // }
}

void AudioProcessorNodeUI::connectOutput(NodeConnectorUI* connection)
{
    // if (!outputConnections.contains(connection))
    // {
    //     outputConnections.add(connection);
    //     this->processorNode->connectOutput(connection->getProcessorNode().get());
    // } 
}

void AudioProcessorNodeUI::disconnectInput(NodeConnectorUI* connection)
{
    // removeFromArray(inputConnections, connection);
    // this->processorNode->disconnectInput(connection->getProcessorNode().get());
}

void AudioProcessorNodeUI::disconnectOutput(NodeConnectorUI* connection)
{
    // removeFromArray(outputConnections, connection);
    // this->processorNode->disconnectOutput(connection->getProcessorNode().get());
}

Array<NodeConnectorUI*> AudioProcessorNodeUI::getOutputConnections()
{
    return outputConnections;
}

Array<NodeConnectorUI*> AudioProcessorNodeUI::getInputConnections()
{
    return inputConnections;
}

Array<NodeConnectorUI*> AudioProcessorNodeUI::getFeedbackConnections()
{
    return feedbackConnections;
}

void AudioProcessorNodeUI::addExistingInput(NodeConnectorUI* inputProcessor)
{
    inputConnections.add(inputProcessor);
}

void AudioProcessorNodeUI::addExistingOutput(NodeConnectorUI* outputProcessor)
{
    outputConnections.add(outputProcessor);
}
