#pragma once
#include <JuceHeader.h>
#include <memory>
#include <utils/Constants.h>

//Forward declarations
class NodeConnectorUI;

#define NodeConnectionPtr std::shared_ptr<NodeConnection>

class NodeConnection
{
public:
    NodeConnection();
    NodeConnection(NodeConnectorUI* start, NodeConnectorUI* end);
    NodeConnection(NodeConnectorUI* start, NodeConnectorUI* end, ConnectionType type);
    ~NodeConnection();
    
    NodeConnectorUI* getStartConnector();
    NodeConnectorUI* getEndConnector();

    void setType(ConnectionType type);
    ConnectionType getType();

private:
    Point<int> startPosition;
    Point<int> endPosition;

    NodeConnectorUI* start = nullptr;
    NodeConnectorUI* end = nullptr;

    ConnectionType connectionType = ConnectionType::Normal;
};