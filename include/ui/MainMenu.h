/*
  ==============================================================================

    Toolbar.h
    Created: 18 Oct 2020 6:03:51pm
    Author:  Will

  ==============================================================================
*/

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
        ///Returns the path to folder/file that should be saved to
        virtual void onSave(std::string filepath) = 0;
        //Returns file that should be loaded
        virtual void onLoad(std::string filepath) = 0;
        //Clears the screen
        virtual void onNewProject() = 0;
        
        virtual void onPresetSelected(Presets presetId) = 0;
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
    void presetSelected(Presets presetId);

    std::unique_ptr<FileChooser> fileChooser;
    std::unique_ptr<MenuBarComponent> menuBar;

    Array<MainMenu::Listener*> listeners;
};