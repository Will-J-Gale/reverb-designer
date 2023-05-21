#include <ui/parameters/ComboBoxParameterUI.h>
#include <utils/Constants.h>

ComboBoxParameterUI::ComboBoxParameterUI()
{
    choices.setBounds(COMBO_X, 0, COMBO_WIDTH, COMBO_HEIGHT);
    name.setBounds(COMBO_TEXT_OFFSET, 0, COMBO_TEXT_WIDTH, COMBO_HEIGHT);
    name.setInterceptsMouseClicks(false, true);
    addAndMakeVisible(choices);
}

void ComboBoxParameterUI::addItem(String item, int itemId)
{
    choices.addItem(item, itemId);
}

int ComboBoxParameterUI::getSelectedItemId()
{
    return choices.getSelectedId();
}

void ComboBoxParameterUI::setSelectedItem(int id)
{
    choices.setSelectedId(id, dontSendNotification);
}

void ComboBoxParameterUI::addListener(ComboBox::Listener* listener)
{
    choices.addListener(listener);
}

Component* ComboBoxParameterUI::getComponent()
{
    return &choices;
}

float ComboBoxParameterUI::getVisibleWidth()
{
    return choices.getWidth() + getNameTextWidth();
}
