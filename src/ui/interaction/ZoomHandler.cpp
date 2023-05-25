#include <ui/interaction/ZoomHandler.h>

ZoomHandler::ZoomHandler(Component* component)
{
    _component = component;
    setOriginalBounds(component->getBounds());
}

void ZoomHandler::zoomIn()
{
    auto transform = _component->getTransform().scaled(_zoomAmount, _zoomAmount);
    
    _component->setTransform(transform);
    _zoomLevel *= _zoomAmount;
    
    updateSize();
}

void ZoomHandler::zoomOut()
{
    auto inverseZoom = 1 / _zoomAmount;
    auto transform = _component->getTransform().scaled(inverseZoom, inverseZoom);
    
    _component->setTransform(transform);
    _zoomLevel *= inverseZoom;
    
    updateSize();
}

void ZoomHandler::reset()
{
    auto transform = _component->getTransform().scaled(1, 1);
    _component->setTransform(transform);
    updateSize();
}

void ZoomHandler::updateSize()
{
    auto width = _originalWidth * (1 / _zoomLevel);
    auto height = _originalHeight * (1 / _zoomLevel);
    _component->setSize(width, height);
}

void ZoomHandler::setOriginalBounds(Rectangle<int> bounds)
{
    _originalWidth = bounds.getWidth();
    _originalHeight = bounds.getHeight();
}
