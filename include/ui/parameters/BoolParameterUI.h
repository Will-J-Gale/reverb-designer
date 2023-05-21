#pragma once
#include <JuceHeader.h>
#include <ui/parameters/BaseParameterUI.h>

class BoolParameterUI : public BaseParameterUI
{
public:
    BoolParameterUI();
    virtual Component* getComponent() override;
    virtual float getVisibleWidth() override;

    void addListener(ToggleButton::Listener* listener);
    void setToggleState(bool state);
private:
    ToggleButton toggle;
};