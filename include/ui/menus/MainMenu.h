#pragma once
#include <JuceHeader.h>
#include <utils/Constants.h>
#include <utils/Presets.h>

class MainMenu : public Component,
                 public MenuBarModel
{
public:

    class Listener
    {
    public:
        virtual void onSave(std::string filepath) = 0;
        virtual void onLoad(std::string filepath) = 0;
        virtual void onNewProject() = 0;
        virtual void onPresetSelected(Presets::Type presetId) = 0;
        virtual void onMacroSelected(File macroFile) = 0;
    };

    MainMenu();

    virtual void resized() override;
    virtual StringArray getMenuBarNames() override;
    virtual PopupMenu getMenuForIndex(int topLevelMenuIndex, const String& menuName) override;
    virtual void menuItemSelected(int menuItemID, int topLevelMenuIndex) override {};

    void addListener(MainMenu::Listener* listener);

private:
    void newProject();
    void saveFile();
    void loadFile();
    void presetSelected(Presets::Type presetId);
    void macroSelected(File macroFile);

private:
    std::unique_ptr<FileChooser> _fileChooser;
    std::unique_ptr<MenuBarComponent> _menuBar;
    Array<MainMenu::Listener*> _listeners;
};