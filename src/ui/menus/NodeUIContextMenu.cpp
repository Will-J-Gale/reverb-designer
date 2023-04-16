/*
  ==============================================================================

    NodeUIContextMenu.cpp
    Created: 3 Oct 2020 12:02:08pm
    Author:  Will

  ==============================================================================
*/

#include <ui/menus/NodeUIContextMenu.h>
#include <utils/Constants.h>

NodeUIContextMenu::NodeUIContextMenu()
{
    addItem((int)NodeUIConextMenuItems::Delete, "Delete", true, false);
    addItem((int)NodeUIConextMenuItems::Reverse, "Reverse", true, false);
    addItem((int)NodeUIConextMenuItems::Duplicate, "Duplicate", true, false);
}
