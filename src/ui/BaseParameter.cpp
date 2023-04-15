/*
  ==============================================================================

    BaseParameter.cpp
    Created: 10 Sep 2020 5:04:29pm
    Author:  Will

  ==============================================================================
*/

#include <ui/BaseParameter.h>
#include "Constants.h"

BaseParameter::BaseParameter()
{
    setBounds(0, 0, PARAMETER_WIDTH, 20);

    name.setJustificationType(Justification::centredLeft);
    name.setBounds(0, 0, PARAMETER_NAME_WIDTH, PARAMETER_NAME_HEIGHT);
    name.setInterceptsMouseClicks(false, false);
    name.setColour(Label::textColourId, Colour::fromString(TEXT_COLOUR));
    name.setFont(Font(FONT_SIZE, Font::bold));
    addAndMakeVisible(name);
}

void BaseParameter::setParameterName(String newName)
{
    name.setText(newName, NotificationType::dontSendNotification);
}

