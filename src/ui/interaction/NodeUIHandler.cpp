/*
  ==============================================================================

    NodeUIHandler.cpp
    Created: 26 Oct 2020 9:45:05pm
    Author:  Will

  ==============================================================================
*/

#include <ui/interaction/NodeUIHandler.h>
#include <ui/nodes/audioProcessors/AudioProcessorNodeUI.h>
#include <dsp/AudioProcessorNode.h>
#include <ui/nodes/audioProcessors/AudioProcessorNodeUIFactory.h>
#include <ui/GraphEditor.h>
#include <dsp/PluginGraph.h>
#include <ui/interaction/ConnectionHandler.h>
#include <ui/nodes/NodeUI.h>
#include <utils/Constants.h>

void NodeUIHandler::initialize(GraphEditor* graphEditor, PluginGraph* pluginGraph)
{
    this->graphEditor = graphEditor;
    this->pluginGraph = pluginGraph;
}

NodeUIPtr NodeUIHandler::createNode(AudioProcessorNodeType type, Point<int> position)
{
    // if (type == AudioProcessorNodeType::Input || type == AudioProcessorNodeType::Output)
    //     jassert("Cannot create input or output processor without providing AudioProcessorNode");

    // auto processorNode = pluginGraph->generateProcessorNode(type);
    // return createNode(type, position, processorNode);

    return std::make_shared<NodeUI>("Test", NodeUIType::AudioProcessor);
}

NodeUIPtr NodeUIHandler::createNode(AudioProcessorNodeType type, Point<int> position, AudioProcessorNodePtr processorNode)
{
    // NodeUIPtr processorNodeUI = AudioProcessorNodeUIFactory::Generate(type);

    // processorNodeUI->setProcessorNode(processorNode);
    // processorNodeUI->setTopLeftPosition(position);

    // return processorNodeUI;
    return nullptr;
}

void NodeUIHandler::initializeProcessor(NodeUIPtr processorNodeUI)
{
    processorNodeUI->addListener(graphEditor);
    graphEditor->nodes.add(processorNodeUI);
    // graphEditor->nodeConnectors.addArray(processorNodeUI->getAllNodeConnectors());
    // graphEditor->addNodeConnectorListeners(processorNodeUI->getAllNodeConnectors());
    graphEditor->addAndMakeVisible(processorNodeUI.get());
}

void NodeUIHandler::deleteProcessor(NodeUI* processorNodeUI)
{
    // Array<NodeConnectorUI*> processorNodes = processorNodeUI->getAllNodeConnectors();
    // for (auto* node : processorNodes)
    // {
    //     graphEditor->connectionHandler.deleteConnection(node);
    //     graphEditor->removeFromArray(graphEditor->nodeConnectors, node);
    // }

    // pluginGraph->deleteProcessorNode(processorNodeUI->getProcessorNode());
    // graphEditor->removeFromArray(graphEditor->nodes, processorNodeUI);
    // pluginGraph->updateProcessPath();
}

void NodeUIHandler::duplicateProcessor(NodeUI* processorNodeUI)
{
    // AudioProcessorNodeType type = processorNodeUI->getType();

    // auto newProcessor = createNode(type, processorNodeUI->getPosition () + Point<int>(DUPLICATE_OFFSET_X, DUPLICATE_OFFSET_Y));
    // auto parameters = processorNodeUI->getAudioParametersAsXml();
    // newProcessor->setAudioParametersFromXml(parameters);
    // delete parameters;
    // initializeProcessor(newProcessor);
}
