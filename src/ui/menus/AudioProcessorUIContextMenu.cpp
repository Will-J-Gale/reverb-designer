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
    addItem((int)AudioProcessorConextMenuItems::Delete, "Delete", true, false);
    addItem((int)AudioProcessorConextMenuItems::Reverse, "Reverse", true, false);
    addItem((int)AudioProcessorConextMenuItems::Duplicate, "Duplicate", true, false);
}
