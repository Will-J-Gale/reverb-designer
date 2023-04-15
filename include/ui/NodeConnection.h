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
#include "Constants.h"

//Forward declarations
class Node;

#define NodeConnectionPtr std::shared_ptr<NodeConnection>

class NodeConnection
{
public:
    NodeConnection();
    NodeConnection(Node* start, Node* end);
    NodeConnection(Node* start, Node* end, ConnectionType type);
    ~NodeConnection();
    
    Node* getStart();
    Node* getEnd();

    void setType(ConnectionType type);
    ConnectionType getType();

private:
    Point<int> startPosition;
    Point<int> endPosition;

    Node* start = nullptr;
    Node* end = nullptr;

    ConnectionType connectionType = ConnectionType::Normal;
};