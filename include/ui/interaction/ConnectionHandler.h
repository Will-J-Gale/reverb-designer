#pragma once
class GraphEditor;
class PluginGraph;
class NodeConnectorUI;
class NodeUI;

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
    NodeUI* handleMacroNode(NodeUI* node, bool isInput);

private:
    GraphEditor* graphEditor;
    PluginGraph* pluginGraph;
};