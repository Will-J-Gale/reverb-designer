/*
  ==============================================================================

    ConnectionHandler.h
    Created: 26 Oct 2020 9:31:34pm
    Author:  Will

  ==============================================================================
*/

#pragma once
class GraphEditor;
class PluginGraph;
class NodeConnectorUI;

class ConnectionHandler
{
public:
    ConnectionHandler(){};
    void initialize(GraphEditor* graphEditor, PluginGraph* pluginGraph);
    void createConnection(NodeConnectorUI* start, NodeConnectorUI* end);
    void createFeedbackConnection(NodeConnectorUI* start, NodeConnectorUI* end);
    void deleteConnection(NodeConnectorUI* nodeConnector);
    bool connectionExists(NodeConnectorUI* start, NodeConnectorUI* end);
    bool isCreatingFeedback(NodeConnectorUI* start, NodeConnectorUI* end);
    bool nodesAreCompatible(NodeConnectorUI* start, NodeConnectorUI* end);

private:
    GraphEditor* graphEditor;
    PluginGraph* pluginGraph;
};