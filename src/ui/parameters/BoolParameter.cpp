/*
  ==============================================================================

    BoolParameter.cpp
    Created: 10 Sep 2020 4:56:04pm
    Author:  Will

  ==============================================================================
*/

#include <ui/nodes/audioProcessors/AudioProcessorNodeUI.h>
#include <ui/parameters/BoolParameter.h>
#include <utils/Constants.h>

BoolParameter::BoolParameter()
{
    name.setTopLeftPosition(30, 0);
    toggle.setBounds(PARAMETER_X_OFFSET, 0, 70, 15);
    setInterceptsMouseClicks(false, true);
    addAndMakeVisible(toggle);
}

Component* BoolParameter::getComponent()
{
    return &toggle;
}

void BoolParameter::addListener(ToggleButton::Listener* listener)
{
    toggle.addListener(listener);
}

void BoolParameter::setToggleState(bool state)
{
    toggle.setToggleState(state, dontSendNotification);
}

