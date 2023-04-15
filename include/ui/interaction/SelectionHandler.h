/*
  ==============================================================================

    Selection.h
    Created: 6 Oct 2020 10:55:14pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class AudioProcessorNode;

class SelectionHandler
{
public:
    void setItems(Array<AudioProcessorNode*> items);

    Array<AudioProcessorNode*> getItems();
    void moveItems(Point<int> velocity);
    void updateItemPositions();
    void clear();
    bool contains(AudioProcessorNode* item);
    bool isEmpty();
    size_t size();
   
private:
    Array<AudioProcessorNode*> items;
    std::map<AudioProcessorNode*, Point<int>> originalPositions;
};