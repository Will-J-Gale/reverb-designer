#pragma once
#include <JuceHeader.h>
#include <ui/parameters/BaseParameterUI.h>

class ComboBoxParameterUI : public BaseParameterUI
{
public:
    ComboBoxParameterUI();
    void addItem(String item, int itemId);
    int getSelectedItemId();
    void setSelectedItem(int id);

    void addListener(ComboBox::Listener* listener);

    virtual Component* getComponent() override;
private:
    ComboBox choices; 
};