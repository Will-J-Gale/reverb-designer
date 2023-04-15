/*
  ==============================================================================

    ConnectionHandler.h
    Created: 26 Oct 2020 9:31:34pm
    Author:  Will

  ==============================================================================
*/

#pragma once
class GraphEditor;
class Node;

class ConnectionHandler
{
public:
    ConnectionHandler(GraphEditor* graphEditor);
    void createConnection(Node* start, Node* end);
    void createFeedbackConnection(Node* start, Node* end);
    void deleteConnection(Node* node);
    bool connectionExists(Node* start, Node* end);
    bool isCreatingFeedback(Node* start, Node* end);
    bool nodesAreCompatible(Node* start, Node* end);

private:
    GraphEditor* graphEditor;
};