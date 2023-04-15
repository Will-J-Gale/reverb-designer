/*
  ==============================================================================

    NodeConnection.cpp
    Created: 13 Sep 2020 3:05:28pm
    Author:  Will

  ==============================================================================
*/

#include <ui/audioProcessor/NodeConnection.h>
#include <ui/audioProcessor/Node.h>
#include <utils/Constants.h>

NodeConnection::NodeConnection()
{
}

NodeConnection::~NodeConnection()
{
    start = nullptr;
    end = nullptr;
}

NodeConnection::NodeConnection(Node* start, Node* end)
{
    this->start = start;
    this->end = end;
}

NodeConnection::NodeConnection(Node* start, Node* end, ConnectionType type)
{
    this->start = start;
    this->end = end;
    this->connectionType = type;
}

Node* NodeConnection::getStart()
{
    return start;
}

Node* NodeConnection::getEnd()
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