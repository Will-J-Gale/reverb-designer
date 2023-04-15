/*
  ==============================================================================

    ConnectionHandler.cpp
    Created: 26 Oct 2020 9:31:34pm
    Author:  Will

  ==============================================================================
*/

#include <ui/ConnectionHandler.h>
#include <memory>
#include <ui/Node.h>
#include <ui/GraphEditor.h>
#include <ui/NodeConnection.h>
#include <dsp/PluginGraph.h>

ConnectionHandler::ConnectionHandler(GraphEditor* graphEditor)
{
    this->graphEditor = graphEditor;
}

void ConnectionHandler::createConnection(Node* start, Node* end)
{
    auto connection = std::make_shared<NodeConnection>(start, end);
    graphEditor->connections.add(connection);

    auto* startProcessorUI = (AudioProcessorUI*)start->getParentComponent();
    auto* endProcessorUI = (AudioProcessorUI*)end->getParentComponent();

    endProcessorUI->connectInput(startProcessorUI);
    startProcessorUI->connectOutput(endProcessorUI);
    graphEditor->pluginGraph->updateProcessPath();
}

void ConnectionHandler::createFeedbackConnection(Node* start, Node* end)
{
    auto connection = std::make_shared<NodeConnection>(start, end);
    graphEditor->connections.add(connection);

    auto* startProcessorUI = (AudioProcessorUI*)start->getParentComponent();
    auto* endProcessorUI = (AudioProcessorUI*)end->getParentComponent();

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
        auto* startProcessor = (AudioProcessorUI*)connection->getStart()->getParentComponent();
        auto* endProcessor = (AudioProcessorUI*)connection->getEnd()->getParentComponent();

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
    auto startProcessingBlock = start->getAttachedProcessor()->getProcessingBlock();
    auto endProcessingBlock = end->getAttachedProcessor()->getProcessingBlock();

    if (startProcessingBlock == nullptr || endProcessingBlock == nullptr) { return false; }
    if (startProcessingBlock == endProcessingBlock) { return true; }

    if (graphEditor->pluginGraph->isCreatingFeedback(endProcessingBlock.get(), startProcessingBlock.get()))
    {
        return true;
    }

    return false;
}

bool ConnectionHandler::nodesAreCompatible(Node* start, Node* end)
{
    return (start->getType() == NodeType::AudioOutput && end->getType() == NodeType::AudioInput);
}
