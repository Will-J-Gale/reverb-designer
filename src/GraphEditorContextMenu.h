/*
  ==============================================================================

    GraphEditorContextMenu.h
    Created: 3 Oct 2020 12:01:36pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class GraphEditorContextMenu : public PopupMenu
{
public:
    GraphEditorContextMenu();
private:
    PopupMenu dspObjectMenu;
};