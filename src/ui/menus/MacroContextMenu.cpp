#include <ui/menus/MacroContextMenu.h>
#include <utils/Constants.h>

MacroContextMenu::MacroContextMenu() : NodeUIContextMenu()
{
    addItem((int)NodeContextMenuItems::Save, "Save", true, false);
}