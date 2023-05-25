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
    Component* _component = nullptr;
    int _originalHeight = 0;
    int _originalWidth = 0;
    float _zoomLevel = 1;
    float _zoomAmount = 1.1;
};