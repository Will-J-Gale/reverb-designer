#include <ui/menus/NodeUIContextMenu.h>
#include <utils/Constants.h>

NodeUIContextMenu::NodeUIContextMenu()
{
    addItem((int)NodeContextMenuItems::Delete, "Delete", true, false);
    addItem((int)NodeContextMenuItems::Reverse, "Reverse", true, false);
    addItem((int)NodeContextMenuItems::Duplicate, "Duplicate", true, false);
}
