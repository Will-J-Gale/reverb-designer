/*
  ==============================================================================

    ConnectionHandler.cpp
    Created: 26 Oct 2020 9:31:34pm
    Author:  Will

  ==============================================================================
*/

#include <memory>
#include <ui/interaction/ConnectionHandler.h>
#include <ui/audioProcessorNodes/AudioProcessorNodeConnectorUI.h>
#include <ui/GraphEditor.h>
#include <ui/audioProcessorNodes/NodeConnection.h>
#include <dsp/PluginGraph.h>

void ConnectionHandler::initialize(GraphEditor* graphEditor, PluginGraph* pluginGraph)
{
    this->graphEditor = graphEditor;
    this->pluginGraph = pluginGraph;
}

void ConnectionHandler::createConnection(AudioProcessorNodeConnectorUI* start, AudioProcessorNodeConnectorUI* end)
{
    auto connection = std::make_shared<NodeConnection>(start, end);
    graphEditor->connections.add(connection);

    auto* startProcessorUI = (AudioProcessorNodeUI*)start->getParentComponent();
    auto* endProcessorUI = (AudioProcessorNodeUI*)end->getParentComponent();

    endProcessorUI->connectInput(startProcessorUI);
    startProcessorUI->connectOutput(endProcessorUI);
    graphEditor->pluginGraph->updateProcessPath();
}

void ConnectionHandler::createFeedbackConnection(AudioProcessorNodeConnectorUI* start, AudioProcessorNodeConnectorUI* end)
{
    auto connection = std::make_shared<NodeConnection>(start, end);
    graphEditor->connections.add(connection);

    auto* startProcessorUI = (AudioProcessorNodeUI*)start->getParentComponent();
    auto* endProcessorUI = (AudioProcessorNodeUI*)end->getParentComponent();

    endProcessorUI->connectFeedbackInput(startProcessorUI);
    graphEditor->pluginGraph->updateProcessPath();
}

void ConnectionHandler::deleteConnection(AudioProcessorNodeConnectorUI* nodeConnector)
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
        auto* startProcessor = (AudioProcessorNodeUI*)connection->getStartConnector()->getParentComponent();
        auto* endProcessor = (AudioProcessorNodeUI*)connection->getEndConnector()->getParentComponent();

        startProcessor->disconnectOutput(endProcessor);
        endProcessor->disconnectInput(startProcessor);

        graphEditor->removeFromArray(graphEditor->connections, connection);
    }

    graphEditor->pluginGraph->updateProcessPath();

    graphEditor->repaint();
}

bool ConnectionHandler::connectionExists(AudioProcessorNodeConnectorUI* start, AudioProcessorNodeConnectorUI* end)
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

bool ConnectionHandler::isCreatingFeedback(AudioProcessorNodeConnectorUI* start, AudioProcessorNodeConnectorUI* end)
{
    auto startProcessorNode = start->getAttachedProcessorUI()->getProcessorNode();
    auto endProcessorNode = end->getAttachedProcessorUI()->getProcessorNode();

    if (startProcessorNode == nullptr || endProcessorNode == nullptr) { return false; }
    if (startProcessorNode == endProcessorNode) { return true; }

    if (graphEditor->pluginGraph->isCreatingFeedback(endProcessorNode.get(), startProcessorNode.get()))
    {
        return true;
    }

    return false;
}

bool ConnectionHandler::nodesAreCompatible(AudioProcessorNodeConnectorUI* start, AudioProcessorNodeConnectorUI* end)
{
    return (start->getType() == NodeType::AudioOutput && end->getType() == NodeType::AudioInput);
}
