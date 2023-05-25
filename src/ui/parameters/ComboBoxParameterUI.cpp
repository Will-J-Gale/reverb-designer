#include <ui/parameters/ComboBoxParameterUI.h>
#include <utils/Constants.h>

ComboBoxParameterUI::ComboBoxParameterUI()
{
    _choices.setBounds(COMBO_X, 0, COMBO_WIDTH, COMBO_HEIGHT);
    _name.setBounds(COMBO_TEXT_OFFSET, 0, COMBO_TEXT_WIDTH, COMBO_HEIGHT);
    _name.setInterceptsMouseClicks(false, true);
    addAndMakeVisible(_choices);
}

void ComboBoxParameterUI::addItem(String item, int itemId)
{
    _choices.addItem(item, itemId);
}

int ComboBoxParameterUI::getSelectedItemId()
{
    return _choices.getSelectedId();
}

void ComboBoxParameterUI::setSelectedItem(int id)
{
    _choices.setSelectedId(id, dontSendNotification);
}

void ComboBoxParameterUI::addListener(ComboBox::Listener* listener)
{
    _choices.addListener(listener);
}

Component* ComboBoxParameterUI::getComponent()
{
    return &_choices;
}

float ComboBoxParameterUI::getVisibleWidth()
{
    return _choices.getWidth() + getNameTextWidth();
}
