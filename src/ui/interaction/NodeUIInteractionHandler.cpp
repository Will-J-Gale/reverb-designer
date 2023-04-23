/*
  ==============================================================================

    NodeUIInteractionHandler.cpp
    Created: 26 Oct 2020 9:45:05pm
    Author:  Will

  ==============================================================================
*/

#include <dsp/AudioProcessorNode.h>
#include <dsp/PluginGraph.h>
#include <ui/interaction/NodeUIInteractionHandler.h>
#include <ui/nodes/audioProcessors/AudioProcessorNodeUI.h>
#include <ui/nodes/audioProcessors/AudioProcessorNodeUIFactory.h>
#include <ui/GraphEditor.h>
#include <ui/interaction/ConnectionHandler.h>
#include <ui/menus/InputModals.h>
#include <ui/nodes/NodeUI.h>
#include <utils/Constants.h>

void NodeUIInteractionHandler::initialize(GraphEditor* graphEditor, PluginGraph* pluginGraph)
{
    this->graphEditor = graphEditor;
    this->pluginGraph = pluginGraph;
}

void NodeUIInteractionHandler::onNodeClicked(NodeUI* node, const MouseEvent& e)
{
    if (!graphEditor->selectionHandler.contains(node))
    {
        graphEditor->selectionHandler.clear();
    }

    graphEditor->repaint();
}

void NodeUIInteractionHandler::onNodeMoved(NodeUI* node, const MouseEvent& e)
{
    auto newE = e.getEventRelativeTo(graphEditor);
    graphEditor->selectionHandler.moveItems(newE.getOffsetFromDragStart());

    graphEditor->repaint();
}

void NodeUIInteractionHandler::onNodeReleased(NodeUI* node, const MouseEvent& e)
{
    graphEditor->selectionHandler.updateItemPositions();
}

void NodeUIInteractionHandler::onNodeContextSelection(NodeUI* node, NodeUIConextMenuItems selection)
{
    if (selection == NodeUIConextMenuItems::Delete)
    {
        if (!graphEditor->selectionHandler.isEmpty())
            graphEditor->deleteSelectedProcessors();
        else
            deleteProcessor(node);
    }
    else if (selection == NodeUIConextMenuItems::Duplicate)
    {
        if (!graphEditor->selectionHandler.isEmpty())
            graphEditor->duplicateSelectedProcessors();
        else
            duplicateProcessor(node);
    }
    else if (selection == NodeUIConextMenuItems::Reverse)
    {
        if (!graphEditor->selectionHandler.isEmpty())
            graphEditor->reverseSelectedProcessors();
        else
            node->reverse();
    }

    graphEditor->repaint();
}
NodeUIPtr NodeUIInteractionHandler::createMacroNode(Point<int> position)
{
    auto macroName = InputModals::runTextInputModal(MACRO_MODAL_TITLE, MACRO_MODAL_MESSAGE, MACRO_DEFAULT_NAME);
    auto macroNode = std::make_shared<AudioProcessorMacroNode>(graphEditor->pluginGraph, macroName);
    macroNode->setTopLeftPosition(position);

    return macroNode;
}
NodeUIPtr NodeUIInteractionHandler::createNode(NodeInstance type, Point<int> position)
{
    auto node = graphEditor->pluginGraph->generateProcessorNode(type);
    return createNode(type, position, node);
}

NodeUIPtr NodeUIInteractionHandler::createNode(NodeInstance type, Point<int> position, AudioProcessorNodePtr processorNode)
{
    NodeUIPtr nodeUI = AudioProcessorNodeUIFactory::Generate(type);

    dynamic_cast<AudioProcessorNodeUI *>(nodeUI.get())->setProcessorNode(processorNode);
    nodeUI->setTopLeftPosition(position);

    return nodeUI;
}

void NodeUIInteractionHandler::initializeNode(NodeUIPtr node)
{
    node->addListener(this);
    graphEditor->nodes.add(node);
    graphEditor->nodeConnectors.addArray(node->getAllNodeConnectors());
    graphEditor->addNodeConnectorListeners(node->getAllNodeConnectors());
    graphEditor->addAndMakeVisible(node.get());
}

void NodeUIInteractionHandler::deleteProcessor(NodeUI *node)
{
    Array<NodeConnectorUI *> nodeConnectors = node->getAllNodeConnectors();
    for (auto *nodeConnector : nodeConnectors)
    {
        graphEditor->connectionHandler.deleteConnection(nodeConnector);
        graphEditor->removeFromArray(graphEditor->nodeConnectors, nodeConnector);
    }

    // pluginGraph->deleteProcessorNode(processorNodeUI->getProcessorNode());
    graphEditor->removeFromArray(graphEditor->nodes, node);
    // pluginGraph->updateProcessPath();
}

void NodeUIInteractionHandler::duplicateProcessor(NodeUI *node)
{
    auto newProcessor = createNode(node->getNodeInstance(), node->getPosition() + Point<int>(DUPLICATE_OFFSET_X, DUPLICATE_OFFSET_Y));
    // auto parameters = node->getAudioParametersAsXml();
    // newProcessor->setAudioParametersFromXml(parameters);
    // delete parameters;
    initializeNode(newProcessor);
}
