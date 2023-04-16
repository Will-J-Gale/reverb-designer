/*
  ==============================================================================

    DragHandler.cpp
    Created: 26 Oct 2020 7:33:09pm
    Author:  Will

  ==============================================================================
*/

#include <ui/interaction/DragHandler.h>

DragHandler::DragHandler(Component* component)
{
    this->component = component;
}

void DragHandler::mouseDown(const MouseEvent& e)
{
    dragOffset = e.getScreenPosition() - component->getScreenPosition();
}

void DragHandler::drag(const MouseEvent& e)
{
    auto pos = e.getScreenPosition() - dragOffset;

    if (component->getParentComponent() != nullptr)
        pos = component->getParentComponent()->getLocalPoint(nullptr, pos);

    component->setTopLeftPosition(pos.x, pos.y);
}
