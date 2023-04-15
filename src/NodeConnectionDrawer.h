/*
  ==============================================================================

    NodeConnectionDrawer.h
    Created: 6 Oct 2020 10:27:16pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Constants.h"

class Node;
class GraphEditor;

class NodeConnectionDrawer
{
public:
    static void drawConnection(Graphics& g, GraphEditor* graphEditor, Node* startNode, Node* endNode);
    static void drawRightAngleConnection(Graphics& g, Point<int> start, Point<int> end);
    static void drawUTurnConnection(Graphics& g, Point<int> start, Point<int> end, bool isLeft);
};