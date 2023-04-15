/*
  ==============================================================================

    BoolParameter.h
    Created: 10 Sep 2020 4:56:04pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <ui/parameters/BaseParameter.h>

class BoolParameter : public BaseParameter
{
public:
    BoolParameter();
    virtual Component* getComponent() override;
    void addListener(ToggleButton::Listener* listener);
    void setToggleState(bool state);
private:
    ToggleButton toggle;
};