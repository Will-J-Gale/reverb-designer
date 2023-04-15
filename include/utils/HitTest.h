/*
  ==============================================================================

    HitTest.h
    Created: 3 Oct 2020 1:16:00pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class HitTest
{
public:
    static bool PointSphereHit(Point<int> pointPosition, Point<int> circlePosition, int circleRadius)
    {
        float distanceSqrd = 
            pow((float)pointPosition.getX() - (float)circlePosition.getX(), 2.0) + 
            pow((float)pointPosition.getY() - (float)circlePosition.getY(), 2.0);
        
        float distance = sqrt(distanceSqrd);

        return distance < circleRadius;
    }

    static bool PointBoxHit(Point<int> point, Rectangle<int>& box)
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

    static bool BoxHit(Rectangle<int>& a, Rectangle<int>& b)
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

};