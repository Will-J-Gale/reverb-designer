/*
  ==============================================================================

    StorageManager.cpp
    Created: 11 Oct 2020 3:26:14pm
    Author:  Will

  ==============================================================================
*/

#include "StorageManager.h"
#include <cstdarg>


void StorageManager::saveXML(std::string filename, std::string xmlString)
{
    auto saveFile = File(filename);
    saveFile.create();
    saveFile.replaceWithText(xmlString);
}

File StorageManager::getStorageDirectory()
{
    auto appDataFolder = File::getSpecialLocation(File::SpecialLocationType::userApplicationDataDirectory);
    auto appDataPath = appDataFolder.getFullPathName();
    auto suffix = std::string("\\") + JucePlugin_Name;
    appDataPath.append(suffix, sizeof(suffix));
    
    auto storageDirectory = File(appDataPath);

    if (!storageDirectory.exists())
    {
        storageDirectory.createDirectory();
    }

    return storageDirectory;
}