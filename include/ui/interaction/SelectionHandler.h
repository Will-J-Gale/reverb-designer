/*
  ==============================================================================

    Selection.h
    Created: 6 Oct 2020 10:55:14pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class AudioProcessorNodeUI;

class SelectionHandler
{
public:
    void setItems(Array<AudioProcessorNodeUI*> items);

    Array<AudioProcessorNodeUI*> getItems();
    void moveItems(Point<int> velocity);
    void updateItemPositions();
    void clear();
    bool contains(AudioProcessorNodeUI* item);
    bool isEmpty();
    size_t size();
   
private:
    Array<AudioProcessorNodeUI*> items;
    std::map<AudioProcessorNodeUI*, Point<int>> originalPositions;
};