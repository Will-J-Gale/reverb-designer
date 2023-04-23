#include <ui/nodes/audioProcessors/AudioProcessorNodeUI.h>
#include <ui/GraphEditor.h>
#include <dsp/Delay.h>


AudioProcessorNodeUI::AudioProcessorNodeUI(String name, NodeInstance nodeInstance) 
    : NodeUI(name, NodeClass::AudioProcessor, nodeInstance)
{
    initialize();
}

AudioProcessorNodeUI::~AudioProcessorNodeUI()
{
}

AudioProcessorNodePtr AudioProcessorNodeUI::getProcessorNode()
{
	return processorNode;
}

void AudioProcessorNodeUI::setProcessorNode(AudioProcessorNodePtr processorNode)
{
    // jassert(processorNode->getNodeInstance() == nodeInstance);

    this->processorNode = processorNode;
    this->setUIParameters();
}

IAudioProcessor* AudioProcessorNodeUI::getAudioProcessor()
{
    return processorNode->getProcessor();
}

std::string AudioProcessorNodeUI::getIdAsString()
{
    return processorNode->getIdAsString();
}

void AudioProcessorNodeUI::connectInput(NodeUI* sourceNode)
{
    if (!inputConnections.contains(sourceNode))
    {
        inputConnections.add(sourceNode);
        auto audioNode = dynamic_cast<AudioProcessorNodeUI*>(sourceNode);
        this->processorNode->connectInput(audioNode->getProcessorNode().get());
    }
}

void AudioProcessorNodeUI::connectFeedbackInput(NodeUI* sourceNode)
{
    if (!feedbackConnections.contains(sourceNode))
    {
        feedbackConnections.add(sourceNode);
        auto audioNode = dynamic_cast<AudioProcessorNodeUI*>(sourceNode);
        this->processorNode->connectFeedbackInput(audioNode->getProcessorNode().get());
    }
}

void AudioProcessorNodeUI::connectOutput(NodeUI* destinationNode)
{
    if (!outputConnections.contains(destinationNode))
    {
        outputConnections.add(destinationNode);

        auto audioNode = dynamic_cast<AudioProcessorNodeUI*>(destinationNode);
        this->processorNode->connectOutput(audioNode->getProcessorNode().get());
    } 
}

void AudioProcessorNodeUI::disconnectInput(NodeUI* sourceNode)
{
    removeFromArray(inputConnections, sourceNode);
    auto audioNode = dynamic_cast<AudioProcessorNodeUI*>(sourceNode);
    this->processorNode->disconnectInput(audioNode->getProcessorNode().get());
}

void AudioProcessorNodeUI::disconnectOutput(NodeUI* destinationNode)
{
    removeFromArray(outputConnections, destinationNode);
    auto audioNode = dynamic_cast<AudioProcessorNodeUI*>(destinationNode);
    this->processorNode->disconnectOutput(audioNode->getProcessorNode().get());
}