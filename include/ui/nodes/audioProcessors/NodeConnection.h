/*
  ==============================================================================

    NodeConnection.h
    Created: 13 Sep 2020 3:05:28pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <memory>
#include <utils/Constants.h>

//Forward declarations
class AudioProcessorNodeConnectorUI;

#define NodeConnectionPtr std::shared_ptr<NodeConnection>

class NodeConnection
{
public:
    NodeConnection();
    NodeConnection(AudioProcessorNodeConnectorUI* start, AudioProcessorNodeConnectorUI* end);
    NodeConnection(AudioProcessorNodeConnectorUI* start, AudioProcessorNodeConnectorUI* end, ConnectionType type);
    ~NodeConnection();
    
    AudioProcessorNodeConnectorUI* getStartConnector();
    AudioProcessorNodeConnectorUI* getEndConnector();

    void setType(ConnectionType type);
    ConnectionType getType();

private:
    Point<int> startPosition;
    Point<int> endPosition;

    AudioProcessorNodeConnectorUI* start = nullptr;
    AudioProcessorNodeConnectorUI* end = nullptr;

    ConnectionType connectionType = ConnectionType::Normal;
};