/*
  ==============================================================================

    HitTest.h
    Created: 3 Oct 2020 1:16:00pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <ui/nodes/NodeUI.h>
#include <ui/nodes/NodeConnectorUI.h>

namespace HitTest
{
    inline bool PointSphereHit(Point<int> pointPosition, Point<int> circlePosition, int circleRadius)
    {
        float distanceSqrd = 
            pow((float)pointPosition.getX() - (float)circlePosition.getX(), 2.0) + 
            pow((float)pointPosition.getY() - (float)circlePosition.getY(), 2.0);
        
        float distance = sqrt(distanceSqrd);

        return distance < circleRadius;
    }

    inline bool PointBoxHit(Point<int> point, Rectangle<int>& box)
    {
        auto boxPos = box.getPosition();

        if (point.x > boxPos.x &&
            point.x < boxPos.x + box.getWidth() &&
            point.y > boxPos.y &&
            point.y < boxPos.y + box.getHeight())
        {
            return true;
        }

        return false;
    }

    inline bool BoxHit(Rectangle<int>& a, Rectangle<int>& b)
    {
        auto aPos = a.getPosition();
        auto bPos = b.getPosition();

        if (aPos.x < bPos.x + b.getWidth() &&
            aPos.x + a.getWidth() > bPos.x &&
            aPos.y < bPos.y + b.getHeight() &&
            aPos.y + a.getHeight() > bPos.y)
        {
            return true;
        }

        return false;
    }

    inline NodeConnectorUI* getNodeConnectorAtPosition(Point<int> screenPos, Array<NodeConnectorUI*>& nodeConnectors)
    {
        for (auto* nodeConnector : nodeConnectors)
        {
            auto nodePos = nodeConnector->getScreenPosition();
            auto nodeRadius = nodeConnector->getBounds().getWidth();
            
            if (PointSphereHit(screenPos, nodePos, nodeRadius))
                return nodeConnector;
        }

        return nullptr;
        }

    inline Array<NodeUI*> getOverlappingNodes(Rectangle<int> bounds, Array<NodeUIPtr>& nodes)
    {
        Array<NodeUI*> overlappingNodes;

        for (auto node : nodes)
        {
            auto otherBounds = node->getBounds();
            if (BoxHit(bounds, otherBounds))
                overlappingNodes.add(node.get());
        }

        return overlappingNodes;
    }
};