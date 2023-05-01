#pragma once
#include <JuceHeader.h>

class ZoomHandler
{
public:
    ZoomHandler(Component* component);
    void zoomIn();
    void zoomOut();
    void reset();
    void updateSize();
    void setOriginalBounds(Rectangle<int> bounds);

private:
    Component* component;
    int originalHeight = 0;
    int originalWidth = 0;
    float zoomLevel = 1;
    float zoomAmount = 1.1;
};