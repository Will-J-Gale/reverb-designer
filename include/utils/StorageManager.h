#pragma once
#include <JuceHeader.h>

namespace StorageManager
{
    inline void saveXML(std::string filename, std::string xmlString)
    {
        auto saveFile = File(filename);
        saveFile.create();
        saveFile.replaceWithText(xmlString);
    }

    inline File getStorageDirectory()
    {
        auto appDataFolder = File::getSpecialLocation(File::SpecialLocationType::userApplicationDataDirectory);
        auto appDataPath = appDataFolder.getFullPathName();
        auto suffix = std::string("/") + JucePlugin_Name;
        appDataPath.append(suffix, sizeof(suffix));
        
        auto storageDirectory = File(appDataPath);

        if (!storageDirectory.exists())
        {
            storageDirectory.createDirectory();
        }

        return storageDirectory;
    }

    inline String getStorageDirectoryPath()
    {
        return getStorageDirectory().getFullPathName();
    }

    inline String getStorageSubdirectoryPath(String subdir)
    {
        String storagePath = getStorageDirectoryPath();
        storagePath += "/" + subdir;

        return storagePath;
    }

    inline Array<File> listDir(String directoryPath)
    {
        File directory = File(directoryPath);
        jassert(directory.isDirectory());
        return directory.findChildFiles(File::TypesOfFileToFind::findFiles, false, FILE_CHOOSER_FILTER, File::FollowSymlinks::no);
    }
}