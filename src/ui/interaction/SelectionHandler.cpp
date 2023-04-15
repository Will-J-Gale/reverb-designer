/*
  ==============================================================================

    Selection.cpp
    Created: 6 Oct 2020 10:55:14pm
    Author:  Will

  ==============================================================================
*/

#include <ui/interaction/SelectionHandler.h>
#include <ui/audioProcessorNodes/AudioProcessorNode.h>

void SelectionHandler::setItems(Array<AudioProcessorNode*> items)
{
    this->items = items;
    
    for (auto item : this->items)
    {
        originalPositions.emplace(item, item->getPosition());
    }
}

void SelectionHandler::moveItems(Point<int> velocity)
{
    for (auto item : items)
    {
        item->setTopLeftPosition(originalPositions[item] + velocity);
    }
}

void SelectionHandler::clear()
{
    items.clear();
    originalPositions.clear();
}

void SelectionHandler::updateItemPositions()
{
    for (auto item : items)
    {
        originalPositions[item] = item->getPosition();
    }
}

bool SelectionHandler::contains(AudioProcessorNode* item)
{
	return items.contains(item);
}

bool SelectionHandler::isEmpty()
{
    return items.isEmpty();
}

size_t SelectionHandler::size()
{
    return items.size();
}

Array<AudioProcessorNode*> SelectionHandler::getItems()
{
    return items;
}
