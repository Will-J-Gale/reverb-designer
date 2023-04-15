/*
  ==============================================================================

    Toolbar.cpp
    Created: 18 Oct 2020 6:03:51pm
    Author:  Will

  ==============================================================================
*/

#include <ui/MainMenu.h>
#include <utils/StorageManager.h>
#include <utils/Constants.h>

MainMenu::MainMenu()
{
    menuBar.reset(new MenuBarComponent(this));
    addAndMakeVisible(menuBar.get());

    setBounds(0, 0, 500, 200);

    fileChooser.reset(new FileChooser(FILE_CHOOSER_TEXT, StorageManager::getStorageDirectory(), "*.xml", true));
}

void MainMenu::resized()
{
    menuBar->setBounds(0, 0, getWidth(), getHeight());
}

StringArray MainMenu::getMenuBarNames()
{
    return { FILE_MENU_TEXT, PRESET_MENU_TEXT };
}

PopupMenu MainMenu::getMenuForIndex(int menuIndex, const String& menuName)
{
    PopupMenu menu;

    if (menuIndex == 0)
    {
        menu.addItem(NEW_PROJECT_TEXT, std::bind(&MainMenu::newProject, this));
        menu.addItem(SAVE_TEXT, std::bind(&MainMenu::saveFile, this));
        menu.addItem(LOAD_TEXT, std::bind(&MainMenu::loadFile, this));
    }
    else if (menuIndex == 1)
    {
        menu.addItem(STEREO_DELAY_PRESET_TEXT, std::bind(&MainMenu::presetSelected, this, Presets::StereoDelay));
        menu.addItem(FEEDFORWARD_COMB_PRESET_TEXT, std::bind(&MainMenu::presetSelected, this, Presets::FeedbackComb));
        menu.addItem(FEEDBACK_COMB_PRESET_TEXT, std::bind(&MainMenu::presetSelected, this, Presets::FeedforwardComb));
        menu.addItem(APF_PRESET_TEXT, std::bind(&MainMenu::presetSelected, this, Presets::APF));
        menu.addItem(SCHROEDER_PRESET_TEXT, std::bind(&MainMenu::presetSelected, this, Presets::SchroederReverb));
        menu.addItem(SPIN_SEMICONDUCTOR_PRESET_TEXT, std::bind(&MainMenu::presetSelected, this, Presets::SpinSemiconductorReverb));
        menu.addItem(FIGURE8_PRESET_TEXT, std::bind(&MainMenu::presetSelected, this, Presets::Figure8Reverb));
    }
    return menu;
}

void MainMenu::addListener(MainMenu::Listener* listener)
{
    listeners.add(listener);
}

void MainMenu::saveFile()
{
    bool fileIsChosen = fileChooser->browseForFileToSave(true);

    if (fileIsChosen)
    {
        auto savePath = fileChooser->getResult().getFullPathName().toStdString();
        for (auto listener : listeners)
        {
            listener->onSave(savePath);
        }
    }
}

void MainMenu::loadFile()
{
    bool fileIsChosen = fileChooser->browseForFileToOpen();

    if (fileIsChosen)
    {
        auto filepath = fileChooser->getResult().getFullPathName().toStdString();
        for (auto listener : listeners)
        {
            listener->onLoad(filepath);
        }
    }
}

void MainMenu::presetSelected(Presets presetId)
{
    for (auto listener : listeners)
    {
        listener->onPresetSelected(presetId);
    }
}

void MainMenu::newProject()
{
    for (auto listener : listeners)
    {
        listener->onNewProject();
    }
}
