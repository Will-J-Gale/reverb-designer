/*
  ==============================================================================

    Selection.h
    Created: 6 Oct 2020 10:55:14pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <ui/nodes/NodeUI.h>

class SelectionHandler
{
public:
    void setItems(Array<NodeUI*> items);

    Array<NodeUI*> getItems();
    void moveItems(Point<int> velocity);
    void updateItemPositions();
    void clear();
    bool contains(NodeUI* item);
    bool isEmpty();
    size_t size();
   
private:
    Array<NodeUI*> items;
    std::map<NodeUI*, Point<int>> originalPositions;
};