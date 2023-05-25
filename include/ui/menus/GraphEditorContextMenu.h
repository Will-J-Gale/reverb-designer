#pragma once
#include <JuceHeader.h>

class GraphEditorContextMenu : public PopupMenu
{
public:
    GraphEditorContextMenu();
private:
    PopupMenu _dspObjectMenu;
};