#pragma once
#include <JuceHeader.h>
#include <utils/Constants.h>

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
        virtual void onPresetSelected(PresetType presetId) = 0;
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
    void presetSelected(PresetType presetId);

    std::unique_ptr<FileChooser> fileChooser;
    std::unique_ptr<MenuBarComponent> menuBar;

    Array<MainMenu::Listener*> listeners;
};