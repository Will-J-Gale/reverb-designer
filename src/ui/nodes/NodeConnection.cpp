#include <ui/nodes/NodeConnection.h>
#include <ui/nodes/NodeConnectorUI.h>
#include <utils/Constants.h>

NodeConnection::NodeConnection()
{
}

NodeConnection::~NodeConnection()
{
    _start = nullptr;
    _end = nullptr;
}

NodeConnection::NodeConnection(NodeConnectorUI* start, NodeConnectorUI* end)
{
    _start = start;
    _end = end;
}

NodeConnection::NodeConnection(NodeConnectorUI* start, NodeConnectorUI* end, ConnectionType connectionType)
{
    _start = start;
    _end = end;
    _connectionType = connectionType;
}

NodeConnectorUI* NodeConnection::getStartConnector()
{
    return _start;
}

NodeConnectorUI* NodeConnection::getEndConnector()
{
    return _end;
}

void NodeConnection::setType(ConnectionType connectionType)
{
    _connectionType = connectionType;
}

ConnectionType NodeConnection::getType()
{
    return _connectionType;
}