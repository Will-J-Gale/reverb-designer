/*
  ==============================================================================

    NodeConnection.cpp
    Created: 13 Sep 2020 3:05:28pm
    Author:  Will

  ==============================================================================
*/

#include <ui/audioProcessorNodes/NodeConnection.h>
#include <ui/audioProcessorNodes/AudioProcessorNodeConnectorUI.h>
#include <utils/Constants.h>

NodeConnection::NodeConnection()
{
}

NodeConnection::~NodeConnection()
{
    start = nullptr;
    end = nullptr;
}

NodeConnection::NodeConnection(AudioProcessorNodeConnectorUI* start, AudioProcessorNodeConnectorUI* end)
{
    this->start = start;
    this->end = end;
}

NodeConnection::NodeConnection(AudioProcessorNodeConnectorUI* start, AudioProcessorNodeConnectorUI* end, ConnectionType type)
{
    this->start = start;
    this->end = end;
    this->connectionType = type;
}

AudioProcessorNodeConnectorUI* NodeConnection::getStartConnector()
{
    return start;
}

AudioProcessorNodeConnectorUI* NodeConnection::getEndConnector()
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