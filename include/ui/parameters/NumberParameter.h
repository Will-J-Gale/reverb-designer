#pragma once
#include <JuceHeader.h>
#include <ui/parameters/BaseParameter.h>

class NumberParameter : public BaseParameter
{
public:
    NumberParameter();

    void setValue(double number);
    void setWidth(int width);
    void addListener(TextEditor::Listener* listener);
    virtual Component* getComponent() override;

private:
    TextEditor value;
};