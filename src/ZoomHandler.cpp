/*
  ==============================================================================

    ZoomHandler.cpp
    Created: 26 Oct 2020 6:56:45pm
    Author:  Will

  ==============================================================================
*/

#include "ZoomHandler.h"

ZoomHandler::ZoomHandler(Component* component)
{
    this->component = component;
    setOriginalBounds(component->getBounds());
}

void ZoomHandler::zoomIn()
{
    auto transform = component->getTransform().scaled(zoomAmount, zoomAmount);
    
    component->setTransform(transform);
    zoomLevel *= zoomAmount;
    
    updateSize();
}

void ZoomHandler::zoomOut()
{
    auto inverseZoom = 1 / zoomAmount;
    auto transform = component->getTransform().scaled(inverseZoom, inverseZoom);
    
    component->setTransform(transform);
    zoomLevel *= inverseZoom;
    
    updateSize();
}

void ZoomHandler::reset()
{
    auto transform = component->getTransform().scaled(1, 1);
    component->setTransform(transform);
    updateSize();
}

void ZoomHandler::updateSize()
{
    auto width = originalWidth * (1 / zoomLevel);
    auto height = originalHeight * (1 / zoomLevel);
    component->setSize(width, height);
}

void ZoomHandler::setOriginalBounds(Rectangle<int> bounds)
{
    originalWidth = bounds.getWidth();
    originalHeight = bounds.getHeight();
}
