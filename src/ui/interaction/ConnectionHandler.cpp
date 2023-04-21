/*
  ==============================================================================

    ConnectionHandler.cpp
    Created: 26 Oct 2020 9:31:34pm
    Author:  Will

  ==============================================================================
*/

#include <memory>
#include <ui/interaction/ConnectionHandler.h>
#include <ui/nodes/NodeConnectorUI.h>
#include <ui/GraphEditor.h>
#include <ui/nodes/NodeConnection.h>
#include <dsp/PluginGraph.h>

void ConnectionHandler::initialize(GraphEditor* graphEditor, PluginGraph* pluginGraph)
{
    this->graphEditor = graphEditor;
    this->pluginGraph = pluginGraph;
}

void ConnectionHandler::createConnection(NodeConnectorUI* start, NodeConnectorUI* end)
{
    auto connection = std::make_shared<NodeConnection>(start, end);
    graphEditor->connections.add(connection);

    auto* startNodeUI = (NodeUI*)start->getParentComponent();
    auto* endNodeUI = (NodeUI*)end->getParentComponent();

    endNodeUI->connectInput(start);
    startNodeUI->connectOutput(end);
    graphEditor->pluginGraph->updateProcessPath();
}

void ConnectionHandler::createFeedbackConnection(NodeConnectorUI* start, NodeConnectorUI* end)
{
    // auto connection = std::make_shared<NodeConnection>(start, end);
    // graphEditor->connections.add(connection);

    // auto* startProcessorUI = (NodeUI*)start->getParentComponent();
    // auto* endProcessorUI = (NodeUI*)end->getParentComponent();

    // endProcessorUI->connectFeedbackInput(startProcessorUI);
    // graphEditor->pluginGraph->updateProcessPath();
}

void ConnectionHandler::deleteConnection(NodeConnectorUI* nodeConnector)
{
    Array<NodeConnection*> nodeConnections;

    for (auto connection : graphEditor->connections)
    {
        if (connection->getEndConnector() == nodeConnector || connection->getStartConnector() == nodeConnector)
        {
            nodeConnections.add(connection.get());
        }
    }

    for (auto connection : nodeConnections)
    {
        auto* startNode = connection->getStartConnector()->getAttachedNodeUI();
        auto* endNode = connection->getEndConnector()->getAttachedNodeUI();

        startNode->disconnectOutput(connection->getEndConnector());
        endNode->disconnectInput(connection->getStartConnector());

        graphEditor->removeFromArray(graphEditor->connections, connection);
    }

    graphEditor->pluginGraph->updateProcessPath();

    graphEditor->repaint();
}

bool ConnectionHandler::connectionExists(NodeConnectorUI* start, NodeConnectorUI* end)
{
    bool exists = false;

    for (auto connection : graphEditor->connections)
    {
        if (connection->getStartConnector() == start && connection->getEndConnector() == end)
        {
            exists = true;
            break;
        }
    }

    return exists;
}

bool ConnectionHandler::isCreatingFeedback(NodeConnectorUI* start, NodeConnectorUI* end)
{
    // auto startProcessorNode = start->getAttachedNodeUI()->getProcessorNode();
    // auto endProcessorNode = end->getAttachedNodeUI()->getProcessorNode();

    // if (startProcessorNode == nullptr || endProcessorNode == nullptr) { return false; }
    // if (startProcessorNode == endProcessorNode) { return true; }

    // if (graphEditor->pluginGraph->isCreatingFeedback(endProcessorNode.get(), startProcessorNode.get()))
    // {
    //     return true;
    // }

    return false;
}

bool ConnectionHandler::nodesAreCompatible(NodeConnectorUI* start, NodeConnectorUI* end)
{
    return (start->getType() == NodeConnectorType::AudioOutput && end->getType() == NodeConnectorType::AudioInput);
}
