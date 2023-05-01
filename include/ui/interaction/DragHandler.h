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