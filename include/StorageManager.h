/*
  ==============================================================================

    StorageManager.h
    Created: 11 Oct 2020 3:26:14pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class StorageManager
{
public:
    static void saveXML(std::string filename, std::string xmlString);
    static File getStorageDirectory();
};