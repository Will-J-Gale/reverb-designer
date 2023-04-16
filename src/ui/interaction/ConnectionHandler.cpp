/*
  ==============================================================================

    ConnectionHandler.cpp
    Created: 26 Oct 2020 9:31:34pm
    Author:  Will

  ==============================================================================
*/

#include <ui/interaction/ConnectionHandler.h>
#include <memory>
#include <ui/audioProcessorNodes/Node.h>
#include <ui/GraphEditor.h>
#include <ui/audioProcessorNodes/NodeConnection.h>
#include <dsp/PluginGraph.h>

ConnectionHandler::ConnectionHandler(GraphEditor* graphEditor)
{
    this->graphEditor = graphEditor;
}

void ConnectionHandler::createConnection(Node* start, Node* end)
{
    auto connection = std::make_shared<NodeConnection>(start, end);
    graphEditor->connections.add(connection);

    auto* startProcessorUI = (AudioProcessorNodeUI*)start->getParentComponent();
    auto* endProcessorUI = (AudioProcessorNodeUI*)end->getParentComponent();

    endProcessorUI->connectInput(startProcessorUI);
    startProcessorUI->connectOutput(endProcessorUI);
    graphEditor->pluginGraph->updateProcessPath();
}

void ConnectionHandler::createFeedbackConnection(Node* start, Node* end)
{
    auto connection = std::make_shared<NodeConnection>(start, end);
    graphEditor->connections.add(connection);

    auto* startProcessorUI = (AudioProcessorNodeUI*)start->getParentComponent();
    auto* endProcessorUI = (AudioProcessorNodeUI*)end->getParentComponent();

    endProcessorUI->connectFeedbackInput(startProcessorUI);
    graphEditor->pluginGraph->updateProcessPath();
}

void ConnectionHandler::deleteConnection(Node* node)
{
    Array<NodeConnection*> nodeConnections;

    for (auto c : graphEditor->connections)
    {
        if (c->getEnd() == node || c->getStart() == node)
        {
            nodeConnections.add(c.get());
        }
    }

    for (auto connection : nodeConnections)
    {
        auto* startProcessor = (AudioProcessorNodeUI*)connection->getStart()->getParentComponent();
        auto* endProcessor = (AudioProcessorNodeUI*)connection->getEnd()->getParentComponent();

        startProcessor->disconnectOutput(endProcessor);
        endProcessor->disconnectInput(startProcessor);

        graphEditor->removeFromArray(graphEditor->connections, connection);
    }

    graphEditor->pluginGraph->updateProcessPath();

    graphEditor->repaint();
}

bool ConnectionHandler::connectionExists(Node* start, Node* end)
{
    bool exists = false;

    for (auto connection : graphEditor->connections)
    {
        if (connection->getStart() == start && connection->getEnd() == end)
        {
            exists = true;
            break;
        }
    }

    return exists;
}

bool ConnectionHandler::isCreatingFeedback(Node* start, Node* end)
{
    auto startProcessorNode = start->getAttachedProcessor()->getProcessorNode();
    auto endProcessorNode = end->getAttachedProcessor()->getProcessorNode();

    if (startProcessorNode == nullptr || endProcessorNode == nullptr) { return false; }
    if (startProcessorNode == endProcessorNode) { return true; }

    if (graphEditor->pluginGraph->isCreatingFeedback(endProcessorNode.get(), startProcessorNode.get()))
    {
        return true;
    }

    return false;
}

bool ConnectionHandler::nodesAreCompatible(Node* start, Node* end)
{
    return (start->getType() == NodeType::AudioOutput && end->getType() == NodeType::AudioInput);
}
