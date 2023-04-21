/*
  ==============================================================================

    NodeUIHandler.cpp
    Created: 26 Oct 2020 9:45:05pm
    Author:  Will

  ==============================================================================
*/

#include <dsp/AudioProcessorNode.h>
#include <ui/interaction/NodeUIHandler.h>
#include <ui/nodes/audioProcessors/AudioProcessorNodeUI.h>
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

NodeUIPtr NodeUIHandler::createNode(NodeInstance type, Point<int> position)
{
    if (type == NodeInstance::Input || type == NodeInstance::Output)
        jassert("Cannot create input or output processor without providing AudioProcessorNode");

    auto processorNode = pluginGraph->generateProcessorNode(type);
    return createNode(type, position, processorNode);

}

NodeUIPtr NodeUIHandler::createNode(NodeInstance type, Point<int> position, AudioProcessorNodePtr processorNode)
{
    NodeUIPtr nodeUI = AudioProcessorNodeUIFactory::Generate(type);

    dynamic_cast<AudioProcessorNodeUI*>(nodeUI.get())->setProcessorNode(processorNode);
    nodeUI->setTopLeftPosition(position);

    return nodeUI;
}

void NodeUIHandler::initializeProcessor(NodeUIPtr processorNodeUI)
{
    processorNodeUI->addListener(graphEditor);
    graphEditor->nodes.add(processorNodeUI);
    graphEditor->nodeConnectors.addArray(processorNodeUI->getAllNodeConnectors());
    graphEditor->addNodeConnectorListeners(processorNodeUI->getAllNodeConnectors());
    graphEditor->addAndMakeVisible(processorNodeUI.get());
}

void NodeUIHandler::deleteProcessor(NodeUI* node)
{
    Array<NodeConnectorUI*> nodeConnectors = node->getAllNodeConnectors();
    for (auto* nodeConnector : nodeConnectors)
    {
        graphEditor->connectionHandler.deleteConnection(nodeConnector);
        graphEditor->removeFromArray(graphEditor->nodeConnectors, nodeConnector);
    }

    // pluginGraph->deleteProcessorNode(processorNodeUI->getProcessorNode());
    graphEditor->removeFromArray(graphEditor->nodes, node);
    // pluginGraph->updateProcessPath();
}

void NodeUIHandler::duplicateProcessor(NodeUI* processorNodeUI)
{
    // NodeType type = processorNodeUI->getType();

    // auto newProcessor = createNode(type, processorNodeUI->getPosition () + Point<int>(DUPLICATE_OFFSET_X, DUPLICATE_OFFSET_Y));
    // auto parameters = processorNodeUI->getAudioParametersAsXml();
    // newProcessor->setAudioParametersFromXml(parameters);
    // delete parameters;
    // initializeProcessor(newProcessor);
}
