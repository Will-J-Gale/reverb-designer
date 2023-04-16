/*
  ==============================================================================

    AudioProcessorUIContextMenu.cpp
    Created: 3 Oct 2020 12:02:08pm
    Author:  Will

  ==============================================================================
*/

#include <ui/menus/AudioProcessorUIContextMenu.h>
#include <utils/Constants.h>

AudioProcessorUIContextMenu::AudioProcessorUIContextMenu()
{
    addItem((int)NodeUIConextMenuItems::Delete, "Delete", true, false);
    addItem((int)NodeUIConextMenuItems::Reverse, "Reverse", true, false);
    addItem((int)NodeUIConextMenuItems::Duplicate, "Duplicate", true, false);
}
