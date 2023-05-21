#pragma once
#include <JuceHeader.h>

class BaseParameterUI : public Component
{
public:
    BaseParameterUI();
    void setParameterName(String newName);
    std::string getId();
    float getNameTextWidth();
    
    virtual Component* getComponent() = 0;
    virtual float getVisibleWidth() = 0;

protected:
    Label name;
    juce::Uuid id;
};