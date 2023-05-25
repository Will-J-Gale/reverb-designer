#include <ui/interaction/DragHandler.h>

DragHandler::DragHandler(Component* component)
{
    _component = component;
}

void DragHandler::mouseDown(const MouseEvent& e)
{
    _dragOffset = e.getScreenPosition() - _component->getScreenPosition();
}

void DragHandler::drag(const MouseEvent& e)
{
    auto pos = e.getScreenPosition() - _dragOffset;

    if (_component->getParentComponent() != nullptr)
        pos = _component->getParentComponent()->getLocalPoint(nullptr, pos);

    _component->setTopLeftPosition(pos.x, pos.y);
}
