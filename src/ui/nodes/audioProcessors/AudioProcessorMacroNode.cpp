#include <dsp/PluginGraph.h>
#include <ui/GraphEditor.h>
#include <ui/nodes/audioProcessors/AudioProcessorMacroNode.h>
#include <ui/nodes/audioProcessors/AudioProcessorNodeUI.h>
#include <utils/XmlUtils.h>

AudioProcessorMacroNode::AudioProcessorMacroNode(PluginGraph* pluginGraph, String name, AudioProcessorNodePtr node) : AudioProcessorNodeUI(name, NodeInstance::Macro, node)
{
    _contextMenu = MacroContextMenu();
    setBounds(0, 0, GAIN_WIDTH, GAIN_HEIGHT);

    addInputConnector();
    addOutputConnector();

    _nameLabel.setBounds(0, 0, GAIN_WIDTH, TEXT_HEIGHT);
    updateNameAndReCenter(name);

    _graphEditor = std::make_shared<GraphEditor>(pluginGraph, this);
    _graphEditor->addInputNode();
    _graphEditor->addOutputNode();
}

AudioProcessorMacroNode::~AudioProcessorMacroNode()
{
}

void AudioProcessorMacroNode::connectInput(NodeUI* sourceNodeUI)
{
    if (!_inputConnections.contains(sourceNodeUI))
    {
        _inputConnections.add(sourceNodeUI);
        auto audioNode = dynamic_cast<AudioProcessorNodeUI*>(sourceNodeUI);
        _processorNode->connectInput(audioNode->getProcessorNode().get());
    }
}

void AudioProcessorMacroNode::connectFeedbackInput(NodeUI* sourceNodeUI)
{
    if (!_feedbackConnections.contains(sourceNodeUI))
    {
        _feedbackConnections.add(sourceNodeUI);
        auto audioNode = dynamic_cast<AudioProcessorNodeUI*>(sourceNodeUI);
        _processorNode->connectFeedbackInput(audioNode->getProcessorNode().get());
    }
}

void AudioProcessorMacroNode::connectOutput(NodeUI* destinationNodeUI)
{
    if (!_outputConnections.contains(destinationNodeUI))
    {
        _outputConnections.add(destinationNodeUI);
        auto audioNodeUI = dynamic_cast<AudioProcessorNodeUI*>(destinationNodeUI);
        
        _processorNode->connectOutput(audioNodeUI->getProcessorNode().get());
    } 
}

void AudioProcessorMacroNode::disconnectInput(NodeUI* sourceNodeUI)
{
    removeFromArray(_inputConnections, sourceNodeUI);
    auto audioNode = dynamic_cast<AudioProcessorNodeUI*>(sourceNodeUI);
    _processorNode->disconnectInput(audioNode->getProcessorNode().get());
}

void AudioProcessorMacroNode::disconnectOutput(NodeUI* destinationNodeUI)
{
    removeFromArray(_outputConnections, destinationNodeUI);
    auto audioNodeUI = dynamic_cast<AudioProcessorNodeUI*>(destinationNodeUI);
    _processorNode->disconnectOutput(audioNodeUI->getProcessorNode().get());
}

void AudioProcessorMacroNode::clearConnections()
{
    _inputConnections.clear();
    _outputConnections.clear();
}

void AudioProcessorMacroNode::mouseDoubleClick(const MouseEvent& e)
{
    auto closeCallback = std::bind(&AudioProcessorMacroNode::closeWindow, this);
    _window = std::make_unique<AudioProcessorMacroWindow>("Macro", closeCallback);
    _window->setContentNonOwned(_graphEditor.get(), true);
}

void AudioProcessorMacroNode::closeWindow()
{
    _window.reset();
}

std::string AudioProcessorMacroNode::getIdAsString()
{
    return _id.toString().toStdString();
}

NodeUIPtr AudioProcessorMacroNode::getInputNodeUI()
{
    return _graphEditor->getInputs()[0];
}
NodeUIPtr AudioProcessorMacroNode::getOutputNodeUI()
{
    return _graphEditor->getOutputs()[0];
}

Array<NodeUIPtr> AudioProcessorMacroNode::getInternalNodes()
{
    return _graphEditor->getNodes();
}

Array<AudioProcessorNodePtr> AudioProcessorMacroNode::getInternalAudioProcessorNodes()
{
    Array<AudioProcessorNodePtr> nodes;

    for(NodeUIPtr& nodeUI : _graphEditor->getNodes())
    {
        if(nodeUI->getNodeInstance() == NodeInstance::Macro)
        {
            auto macroNode = static_cast<AudioProcessorMacroNode*>(nodeUI.get());
            nodes.addArray(macroNode->getInternalAudioProcessorNodes());
        }
        else
        {
            nodes.add(static_cast<AudioProcessorNodeUI*>(nodeUI.get())->getProcessorNode());
        }

    }

    return nodes;
}

XmlElement* AudioProcessorMacroNode::toXml()
{
    XmlElement* macroXml = XmlUtils::generateNodeXml(this);
    XmlElement* macroNodesXml = _graphEditor->toXml();
    macroXml->addChildElement(macroNodesXml);
    return macroXml;
}

void AudioProcessorMacroNode::fromXml(XmlElement* xml)
{
    _name = xml->getChildByName(NAME_TAG)->getAllSubText().toStdString();
    int x = xml->getChildByName(X_TAG)->getAllSubText().getIntValue();
    int y = xml->getChildByName(Y_TAG)->getAllSubText().getIntValue();

    updateNameAndReCenter(_name);
    setTopLeftPosition(x, y);
    _graphEditor->clear();
    // _graphEditor->addInputNode();
    // _graphEditor->addOutputNode();
    _graphEditor->fromXml(xml->getChildByName(PLUGIN_STATE_TAG));
}