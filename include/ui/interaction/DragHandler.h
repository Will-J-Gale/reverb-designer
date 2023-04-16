/*
  ==============================================================================

    DragHandler.h
    Created: 26 Oct 2020 7:33:09pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

class DragHandler
{
public:
    DragHandler(Component* component);
    void mouseDown(const MouseEvent& e);
    void drag(const MouseEvent& e);

private:
    Component* component;
    Point<int> dragOffset;
};