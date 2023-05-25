#include <ui/interaction/ConnectionHandler.h>
#include <ui/nodes/NodeConnectorUI.h>
#include <ui/GraphEditor.h>
#include <ui/nodes/NodeConnection.h>
#include <dsp/PluginGraph.h>
#include <ui/nodes/audioProcessors/AudioProcessorMacroNode.h>
#include <ui/nodes/NodeUI.h>

void ConnectionHandler::initialize(GraphEditor* graphEditor, PluginGraph* pluginGraph)
{
    _graphEditor = graphEditor;
    _pluginGraph = pluginGraph;
}

void ConnectionHandler::createConnection(NodeConnectorUI* start, NodeConnectorUI* end)
{
    auto connection = std::make_shared<NodeConnection>(start, end);
    _graphEditor->_connections.add(connection);

    auto* startNodeUI = (NodeUI*)start->getParentComponent();
    auto* endNodeUI = (NodeUI*)end->getParentComponent();

    startNodeUI = handleMacroNode(startNodeUI, false);
    endNodeUI = handleMacroNode(endNodeUI, true);

    endNodeUI->connectInput(startNodeUI);
    startNodeUI->connectOutput(endNodeUI);
    _graphEditor->_pluginGraph->updateProcessPath();
}

void ConnectionHandler::createFeedbackConnection(NodeConnectorUI* start, NodeConnectorUI* end)
{
    auto connection = std::make_shared<NodeConnection>(start, end);
    _graphEditor->_connections.add(connection);

    auto* startNodeUI = (NodeUI*)start->getParentComponent();
    auto* endNodeUI = (NodeUI*)end->getParentComponent();

    startNodeUI = handleMacroNode(startNodeUI, false);
    endNodeUI = handleMacroNode(endNodeUI, true);

    endNodeUI->connectFeedbackInput(startNodeUI);
    _graphEditor->_pluginGraph->updateProcessPath();
}

void ConnectionHandler::deleteConnection(NodeConnectorUI* nodeConnector)
{
    Array<NodeConnection*> nodeConnections;

    for (auto connection : _graphEditor->_connections)
    {
        if (connection->getEndConnector() == nodeConnector || connection->getStartConnector() == nodeConnector)
        {
            nodeConnections.add(connection.get());
        }
    }

    for (auto connection : nodeConnections)
    {
        auto* startNodeUI = connection->getStartConnector()->getAttachedNodeUI();
        auto* endNodeUI = connection->getEndConnector()->getAttachedNodeUI();

        startNodeUI = handleMacroNode(startNodeUI, false);
        endNodeUI = handleMacroNode(endNodeUI, true);

        startNodeUI->disconnectOutput(endNodeUI);
        endNodeUI->disconnectInput(startNodeUI);
        _graphEditor->removeFromArray(_graphEditor->_connections, connection);
    }

    _graphEditor->_pluginGraph->updateProcessPath();
    _graphEditor->repaint();
}

bool ConnectionHandler::connectionExists(NodeConnectorUI* start, NodeConnectorUI* end)
{
    bool exists = false;

    for (auto connection : _graphEditor->_connections)
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
    auto startNodeUI = start->getAttachedNodeUI();
    auto endNodeUI = end->getAttachedNodeUI();

    startNodeUI = handleMacroNode(startNodeUI, false);
    endNodeUI = handleMacroNode(endNodeUI, false);

    auto startProcessorNode = dynamic_cast<AudioProcessorNodeUI*>(startNodeUI)->getProcessorNode();
    auto endProcessorNode = dynamic_cast<AudioProcessorNodeUI*>(endNodeUI)->getProcessorNode();

    if (startProcessorNode == nullptr || endProcessorNode == nullptr) { return false; }
    if (startProcessorNode == endProcessorNode) { return true; }

    if (_graphEditor->_pluginGraph->isCreatingFeedback(endProcessorNode.get(), startProcessorNode.get()))
    {
        return true;
    }

    return false;
}

bool ConnectionHandler::nodesAreCompatible(NodeConnectorUI* start, NodeConnectorUI* end)
{
    return (start->getType() == NodeConnectorType::AudioOutput && end->getType() == NodeConnectorType::AudioInput);
}

NodeUI* ConnectionHandler::handleMacroNode(NodeUI* node, bool isInput)
{
    if(node->getNodeInstance() == NodeInstance::Macro)
    {
        auto macroNode = dynamic_cast<AudioProcessorMacroNode*>(node);
        
        if(isInput)
            return macroNode->getInputNodeUI().get();
        else
            return macroNode->getOutputNodeUI().get(); 
    }

    return node;
}