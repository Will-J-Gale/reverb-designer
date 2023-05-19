#pragma once
#include <ui/menus/MainMenu.h>
#include <utils/Presets.h>

class MainGraphEditor;

class MainMenuInteractionHandler : public MainMenu::Listener
{
public:
    MainMenuInteractionHandler(MainGraphEditor* mainGraphEditor);
    void onSave(std::string filepath);
    void onLoad(std::string filepath);
    void onNewProject();
    void onPresetSelected(Presets::Type presetId);
    void onTestReset();

private:
    MainGraphEditor* mainGraphEditor;
};