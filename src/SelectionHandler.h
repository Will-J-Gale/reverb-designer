/*
  ==============================================================================

    Selection.h
    Created: 6 Oct 2020 10:55:14pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class AudioProcessorUI;

class SelectionHandler
{
public:
    void setItems(Array<AudioProcessorUI*> items);

    Array<AudioProcessorUI*> getItems();
    void moveItems(Point<int> velocity);
    void updateItemPositions();
    void clear();
    bool contains(AudioProcessorUI* item);
    bool isEmpty();
    size_t size();
   
private:
    Array<AudioProcessorUI*> items;
    std::map<AudioProcessorUI*, Point<int>> originalPositions;
};