#include <ui/menus/MainMenu.h>
#include <utils/StorageManager.h>
#include <utils/Constants.h>

MainMenu::MainMenu()
{
    menuBar.reset(new MenuBarComponent(this));
    addAndMakeVisible(menuBar.get());

    setBounds(0, 0, 500, 200);

    fileChooser.reset(new FileChooser(FILE_CHOOSER_TEXT, StorageManager::getStorageDirectory(), FILE_CHOOSER_FILTER, true));
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
        menu.addItem(STEREO_DELAY_PRESET_TEXT, std::bind(&MainMenu::presetSelected, this, Presets::Type::STEREO_DELAY));
        menu.addItem(FEEDFORWARD_COMB_PRESET_TEXT, std::bind(&MainMenu::presetSelected, this, Presets::Type::FEEDFORWARD_COMB));
        menu.addItem(FEEDBACK_COMB_PRESET_TEXT, std::bind(&MainMenu::presetSelected, this, Presets::Type::FEEDBACK_COMB));
        menu.addItem(APF_PRESET_TEXT, std::bind(&MainMenu::presetSelected, this, Presets::Type::APF));
        // menu.addItem(FEEDBACK_COMB_PRESET_TEXT, std::bind(&MainMenu::presetSelected, this, PresetType::FeedforwardComb));
        // menu.addItem(APF_PRESET_TEXT, std::bind(&MainMenu::presetSelected, this, PresetType::APF));
        // menu.addItem(SCHROEDER_PRESET_TEXT, std::bind(&MainMenu::presetSelected, this, PresetType::SchroederReverb));
        // menu.addItem(SPIN_SEMICONDUCTOR_PRESET_TEXT, std::bind(&MainMenu::presetSelected, this, PresetType::SpinSemiconductorReverb));
        // menu.addItem(FIGURE8_PRESET_TEXT, std::bind(&MainMenu::presetSelected, this, PresetType::Figure8Reverb));
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
        auto file = fileChooser->getResult();
        auto originalPath = file.getFullPathName();

        if(file.getFileExtension().length() == 0)
            originalPath.append(SAVE_FILE_EXT, SAVE_FILE_EXT.length());
        else
            originalPath = originalPath.replaceFirstOccurrenceOf(file.getFileExtension(), SAVE_FILE_EXT);

        std::string savePath = originalPath.toStdString();

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

void MainMenu::presetSelected(Presets::Type presetId)
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