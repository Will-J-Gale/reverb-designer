#pragma once
#include <JuceHeader.h>

class BaseParameter : public Component
{
public:
    BaseParameter();
    void setParameterName(String newName);
    std::string getId();
    virtual Component* getComponent() = 0;

protected:
    Label name;
    juce::Uuid id;
};