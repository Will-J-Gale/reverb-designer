#pragma once
#include <ui/menus/MainMenu.h>

class GraphEditor;

class MainMenuInteractionHandler : public MainMenu::Listener
{
public:
    MainMenuInteractionHandler(GraphEditor* graphEditor);
    void onSave(std::string filepath);
    void onLoad(std::string filepath);
    void onNewProject();
    void onPresetSelected(PresetType presetId);

private:
    GraphEditor* graphEditor;

};