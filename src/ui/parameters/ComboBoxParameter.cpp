#include <ui/parameters/ComboBoxParameter.h>
#include <utils/Constants.h>

ComboBoxParameter::ComboBoxParameter()
{
    choices.setBounds(COMBO_X, 0, COMBO_WIDTH, COMBO_HEIGHT);
    name.setBounds(COMBO_TEXT_OFFSET, 0, COMBO_TEXT_WIDTH, COMBO_HEIGHT);
    name.setInterceptsMouseClicks(false, true);
    addAndMakeVisible(choices);
}

void ComboBoxParameter::addItem(String item, int itemId)
{
    choices.addItem(item, itemId);
}

int ComboBoxParameter::getSelectedItemId()
{
    return choices.getSelectedId();
}

void ComboBoxParameter::setSelectedItem(int id)
{
    choices.setSelectedId(id, dontSendNotification);
}

void ComboBoxParameter::addListener(ComboBox::Listener* listener)
{
    choices.addListener(listener);
}

Component* ComboBoxParameter::getComponent()
{
    return &choices;
}
