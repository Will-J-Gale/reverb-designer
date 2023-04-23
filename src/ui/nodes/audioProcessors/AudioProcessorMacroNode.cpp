#include <dsp/PluginGraph.h>
#include <ui/nodes/audioProcessors/AudioProcessorMacroNode.h>
#include <ui/GraphEditor.h>
#include <ui/nodes/audioProcessors/AudioProcessorNodeUI.h>

AudioProcessorMacroNode::AudioProcessorMacroNode(PluginGraph* pluginGraph, String name) : AudioProcessorNodeUI(name, NodeInstance::Macro)
{
    setBounds(0, 0, GAIN_WIDTH, GAIN_HEIGHT);

    addInputConnector();
    addOutputConnector();

    nameLabel.setBounds(0, 0, GAIN_WIDTH, TEXT_HEIGHT);
    updateNameAndReCenter(name);

    graphEditor = std::make_shared<GraphEditor>(pluginGraph);
    graphEditor->addInputNode();
    graphEditor->addOutputNode();
}

AudioProcessorMacroNode::~AudioProcessorMacroNode()
{
}

void AudioProcessorMacroNode::connectInput(NodeUI* sourceNodeUI)
{
    if (!inputConnections.contains(sourceNodeUI))
    {
        inputConnections.add(sourceNodeUI);
        auto audioNode = dynamic_cast<AudioProcessorNodeUI*>(sourceNodeUI);
        this->processorNode->connectInput(audioNode->getProcessorNode().get());
    }
}

void AudioProcessorMacroNode::connectFeedbackInput(NodeUI* sourceNodeUI)
{
    if (!feedbackConnections.contains(sourceNodeUI))
    {
        feedbackConnections.add(sourceNodeUI);
        auto audioNode = dynamic_cast<AudioProcessorNodeUI*>(sourceNodeUI);
        this->processorNode->connectFeedbackInput(audioNode->getProcessorNode().get());
    }
}

void AudioProcessorMacroNode::connectOutput(NodeUI* destinationNodeUI)
{
    if (!outputConnections.contains(destinationNodeUI))
    {
        outputConnections.add(destinationNodeUI);
        auto audioNodeUI = dynamic_cast<AudioProcessorNodeUI*>(destinationNodeUI);
        
        this->processorNode->connectOutput(audioNodeUI->getProcessorNode().get());
    } 
}

void AudioProcessorMacroNode::disconnectInput(NodeUI* sourceNodeUI)
{
    removeFromArray(inputConnections, sourceNodeUI);
    auto audioNode = dynamic_cast<AudioProcessorNodeUI*>(sourceNodeUI);
    this->processorNode->disconnectInput(audioNode->getProcessorNode().get());
}

void AudioProcessorMacroNode::disconnectOutput(NodeUI* destinationNodeUI)
{
    removeFromArray(outputConnections, destinationNodeUI);
    auto audioNodeUI = dynamic_cast<AudioProcessorNodeUI*>(destinationNodeUI);
    this->processorNode->disconnectOutput(audioNodeUI->getProcessorNode().get());
}

void AudioProcessorMacroNode::mouseDoubleClick(const MouseEvent& e)
{
    auto closeCallback = std::bind(&AudioProcessorMacroNode::closeWindow, this);
    window = std::make_unique<AudioProcessorMacroWindow>("Macro", closeCallback);
    window->setContentNonOwned(graphEditor.get(), true);
}

void AudioProcessorMacroNode::closeWindow()
{
    window.reset();
}

std::string AudioProcessorMacroNode::getIdAsString()
{
    return id.toString().toStdString();
}

NodeUIPtr AudioProcessorMacroNode::getInputNodeUI()
{
    return graphEditor->getInputs()[0];
}
NodeUIPtr AudioProcessorMacroNode::getOutputNodeUI()
{
    return graphEditor->getOutputs()[0];
}