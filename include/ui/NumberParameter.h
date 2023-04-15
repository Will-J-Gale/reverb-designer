/*
  ==============================================================================

    NumberParameter.h
    Created: 10 Sep 2020 4:24:16pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <ui/BaseParameter.h>

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