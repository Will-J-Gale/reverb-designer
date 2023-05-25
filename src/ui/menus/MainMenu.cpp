#include <ui/menus/MainMenu.h>
#include <utils/StorageManager.h>
#include <utils/Constants.h>
#include <utils/StorageManager.h>

MainMenu::MainMenu()
{
    _menuBar.reset(new MenuBarComponent(this));
    addAndMakeVisible(_menuBar.get());

    setBounds(0, 0, 500, 200);

    _fileChooser.reset(new FileChooser(FILE_CHOOSER_TEXT, StorageManager::getStorageDirectory(), FILE_CHOOSER_FILTER, true));
}

void MainMenu::resized()
{
    _menuBar->setBounds(0, 0, getWidth(), getHeight());
}

StringArray MainMenu::getMenuBarNames()
{
    return { FILE_MENU_TEXT, PRESET_MENU_TEXT, MACRO_MENU_TEXT };
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
        menu.addItem(SCHROEDER_PRESET_TEXT, std::bind(&MainMenu::presetSelected, this, Presets::Type::SCHROEDER_REVERB));
        menu.addItem(ALLPASS_LOOP_PRESET_TEXT, std::bind(&MainMenu::presetSelected, this, Presets::Type::ALLPASS_LOOP));
        menu.addItem(FIGURE8_PRESET_TEXT, std::bind(&MainMenu::presetSelected, this, Presets::Type::FIGURE_8_REVERB));
    }
    else if (menuIndex == 2)
    {
        String macroFolderPath = StorageManager::getStorageSubdirectoryPath(MACRO_DIR_NAME);
        Array<File> macroFiles = StorageManager::listDir(macroFolderPath);

        for(File& file : macroFiles)
        {
            menu.addItem(file.getFileNameWithoutExtension(), std::bind(&MainMenu::macroSelected, this, file));
        }
    }

    return menu;
}

void MainMenu::addListener(MainMenu::Listener* listener)
{
    _listeners.add(listener);
}

void MainMenu::saveFile()
{
    bool fileIsChosen = _fileChooser->browseForFileToSave(true);

    if (fileIsChosen)
    {
        auto file = _fileChooser->getResult();
        auto originalPath = file.getFullPathName();

        if(file.getFileExtension().length() == 0)
            originalPath.append(SAVE_FILE_EXT, (size_t)SAVE_FILE_EXT.length());
        else
            originalPath = originalPath.replaceFirstOccurrenceOf(file.getFileExtension(), SAVE_FILE_EXT);

        std::string savePath = originalPath.toStdString();

        for (auto listener : _listeners)
        {
            listener->onSave(savePath);
        }
    }
}

void MainMenu::loadFile()
{
    bool fileIsChosen = _fileChooser->browseForFileToOpen();

    if (fileIsChosen)
    {
        auto filepath = _fileChooser->getResult().getFullPathName().toStdString();
        for (auto listener : _listeners)
        {
            listener->onLoad(filepath);
        }
    }
}

void MainMenu::presetSelected(Presets::Type presetId)
{
    for (auto listener : _listeners)
    {
        listener->onPresetSelected(presetId);
    }
}

void MainMenu::newProject()
{
    for (auto listener : _listeners)
    {
        listener->onNewProject();
    }
}

void MainMenu::macroSelected(File macroFile)
{
    for (auto listener : _listeners)
    {
        listener->onMacroSelected(macroFile);
    }
}