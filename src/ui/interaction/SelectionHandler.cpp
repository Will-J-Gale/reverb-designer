#include <ui/interaction/SelectionHandler.h>

void SelectionHandler::setItems(Array<NodeUI*> items)
{
    _items = items;
    
    for (auto item : _items)
    {
        _originalPositions.emplace(item, item->getPosition());
    }
}

void SelectionHandler::moveItems(Point<int> velocity)
{
    for (auto item : _items)
    {
        item->setTopLeftPosition(_originalPositions[item] + velocity);
    }
}

void SelectionHandler::clear()
{
    _items.clear();
    _originalPositions.clear();
}

void SelectionHandler::updateItemPositions()
{
    for (auto item : _items)
    {
        _originalPositions[item] = item->getPosition();
    }
}

bool SelectionHandler::contains(NodeUI* item)
{
	return _items.contains(item);
}

bool SelectionHandler::isEmpty()
{
    return _items.isEmpty();
}

size_t SelectionHandler::size()
{
    return _items.size();
}

Array<NodeUI*> SelectionHandler::getItems()
{
    return _items;
}
