/*
  ==============================================================================

    ComboBoxParameter.h
    Created: 10 Sep 2020 5:24:27pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <ui/parameters/BaseParameter.h>

class ComboBoxParameter : public BaseParameter
{
public:
    ComboBoxParameter();
    void addItem(String item, int itemId);
    int getSelectedItemId();
    void setSelectedItem(int id);

    void addListener(ComboBox::Listener* listener);

    virtual Component* getComponent() override;
private:
    ComboBox choices; 
};