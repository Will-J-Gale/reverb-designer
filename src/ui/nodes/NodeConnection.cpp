/*
  ==============================================================================

    NodeConnection.cpp
    Created: 13 Sep 2020 3:05:28pm
    Author:  Will

  ==============================================================================
*/

#include <ui/nodes/NodeConnection.h>
#include <ui/nodes/NodeConnectorUI.h>
#include <utils/Constants.h>

NodeConnection::NodeConnection()
{
}

NodeConnection::~NodeConnection()
{
    start = nullptr;
    end = nullptr;
}

NodeConnection::NodeConnection(NodeConnectorUI* start, NodeConnectorUI* end)
{
    this->start = start;
    this->end = end;
}

NodeConnection::NodeConnection(NodeConnectorUI* start, NodeConnectorUI* end, ConnectionType type)
{
    this->start = start;
    this->end = end;
    this->connectionType = type;
}

NodeConnectorUI* NodeConnection::getStartConnector()
{
    return start;
}

NodeConnectorUI* NodeConnection::getEndConnector()
{
    return end;
}

void NodeConnection::setType(ConnectionType type)
{
    connectionType = type;
}

ConnectionType NodeConnection::getType()
{
    return connectionType;
}