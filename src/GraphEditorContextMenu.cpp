/*
  ==============================================================================

    GraphEditorContextMenu.cpp
    Created: 3 Oct 2020 12:01:36pm
    Author:  Will

  ==============================================================================
*/

#include "GraphEditorContextMenu.h"
#include "Constants.h"

GraphEditorContextMenu::GraphEditorContextMenu()
{
    dspObjectMenu.addItem((int)GraphEditorContextMenuItems::Gain, "Gain", true, false);
    dspObjectMenu.addItem((int)GraphEditorContextMenuItems::Delay, "Delay", true, false);
    dspObjectMenu.addItem((int)GraphEditorContextMenuItems::Filter, "Filter", true, false);
    dspObjectMenu.addItem((int)GraphEditorContextMenuItems::APF, "APF", true, false);
    dspObjectMenu.addItem((int)GraphEditorContextMenuItems::Comb, "Comb", true, false);
    dspObjectMenu.addItem((int)GraphEditorContextMenuItems::ModDelay, "Mod Delay", true, false);
    dspObjectMenu.addItem((int)GraphEditorContextMenuItems::NestedApf, "Nested APF", true, false);

    addSubMenu("Add DSP Object", dspObjectMenu);

    addItem((int)GraphEditorContextMenuItems::Duplicate, "Duplicate Seleciton", true, false);
}
