#include <ui/menus/GraphEditorContextMenu.h>
#include <utils/Constants.h>

GraphEditorContextMenu::GraphEditorContextMenu()
{
    _dspObjectMenu.addItem((int)GraphEditorContextMenuItems::Gain, "Gain", true, false);
    _dspObjectMenu.addItem((int)GraphEditorContextMenuItems::Delay, "Delay", true, false);
    _dspObjectMenu.addItem((int)GraphEditorContextMenuItems::Filter, "Filter", true, false);
    _dspObjectMenu.addItem((int)GraphEditorContextMenuItems::APF, "APF", true, false);
    _dspObjectMenu.addItem((int)GraphEditorContextMenuItems::Comb, "Comb", true, false);
    _dspObjectMenu.addItem((int)GraphEditorContextMenuItems::ModDelay, "Mod Delay", true, false);
    _dspObjectMenu.addItem((int)GraphEditorContextMenuItems::NestedApf, "Nested APF", true, false);

    addSubMenu("Add DSP Object", _dspObjectMenu);

    addItem((int)GraphEditorContextMenuItems::Duplicate, "Duplicate Seleciton", true, false);
    addItem((int)GraphEditorContextMenuItems::Macro, "Create audio processor macro", true, false);
}
