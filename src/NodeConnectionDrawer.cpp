/*
  ==============================================================================

    NodeConnectionDrawer.cpp
    Created: 6 Oct 2020 10:32:38pm
    Author:  Will

  ==============================================================================
*/

#include "NodeConnectionDrawer.h"
#include "AudioProcessorUI.h"
#include "GraphEditor.h"

void NodeConnectionDrawer::drawConnection(Graphics& g, GraphEditor* graphEditor, Node* startNode, Node* endNode)
{
    auto fromPos = startNode->getScreenPosition();
    auto toPos = endNode->getScreenPosition();

    auto from = graphEditor->getLocalPoint(nullptr, fromPos);
    auto to = graphEditor->getLocalPoint(nullptr, toPos);

    int x1 = from.getX() + NODE_RADIUS;
    int y1 = from.getY() + NODE_RADIUS;
    int x2 = to.getX() + NODE_RADIUS;
    int y2 = to.getY() + NODE_RADIUS;

    auto startProcessor = (AudioProcessorUI*)(startNode->getParentComponent());
    auto endProcessor = (AudioProcessorUI*)(endNode->getParentComponent());

    bool startReversed = startProcessor->isReversed();
    bool endReversed = endProcessor->isReversed();

    if (startReversed == endReversed)
    {
        drawRightAngleConnection(g, Point<int>(x1, y1), Point<int>(x2, y2));
    }
    else if (startReversed && !endReversed)
    {
        drawUTurnConnection(g, Point<int>(x1, y1), Point<int>(x2, y2), true);
    }
    else if (!startReversed && endReversed)
    {
        drawUTurnConnection(g, Point<int>(x1, y1), Point<int>(x2, y2), false);
    }
}

void NodeConnectionDrawer::drawRightAngleConnection(Graphics& g, Point<int> start, Point<int> end)
{
    auto center = start + ((end - start) / 2);

    Line<float> line = Line<float>();
    line.setStart(center.x, end.y);
    line.setEnd(end.x, end.y);

    g.drawLine(start.x, start.y, center.x, start.y, CONNECTION_THICKNESS);
    g.drawLine(center.x, start.y, center.x, end.y, CONNECTION_THICKNESS);
    g.drawArrow(line, CONNECTION_THICKNESS, ARROW_SIZE, ARROW_SIZE);
}

void NodeConnectionDrawer::drawUTurnConnection(Graphics& g, Point<int> start, Point<int> end, bool isLeft)
{
    int vertLineX = isLeft ? fmin(start.x, end.x) : fmax(start.x, end.x);
    auto gap = isLeft ? -CONNECTION_GAP : CONNECTION_GAP;

    Line<float> line = Line<float>();
    line.setStart(vertLineX + gap, end.y);
    line.setEnd(end.x, end.y);

    g.drawLine(start.x, start.y, vertLineX + gap, start.y, CONNECTION_THICKNESS);
    g.drawLine(vertLineX + gap, start.y, vertLineX + gap, end.y, CONNECTION_THICKNESS);
    g.drawArrow(line, CONNECTION_THICKNESS, ARROW_SIZE, ARROW_SIZE);
}
