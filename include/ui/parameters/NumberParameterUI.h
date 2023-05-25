#pragma once
#include <JuceHeader.h>
#include <ui/parameters/BaseParameterUI.h>

class NumberParameterUI : public BaseParameterUI
{
public:
    NumberParameterUI();

    void setValue(double number);
    void setWidth(int width);
    void addListener(TextEditor::Listener* listener);
    virtual Component* getComponent() override;
    virtual float getVisibleWidth() override;

private:
    TextEditor _value;
};