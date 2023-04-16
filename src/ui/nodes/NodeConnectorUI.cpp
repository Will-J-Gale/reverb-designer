/*
  ==============================================================================

    Node.cpp
    Created: 10 Sep 2020 6:53:28pm
    Author:  Will

  ==============================================================================
*/

#include <ui/nodes/NodeConnectorUI.h>
#include <utils/Constants.h>
#include <ui/GraphEditor.h>
#include <ui/nodes/NodeUI.h>

NodeConnectorUI::NodeConnectorUI()
{
    initialize();
}

NodeConnectorUI::NodeConnectorUI(NodeUI* attachedProcessorUI)
{
    this->attachedProcessorUI = attachedProcessorUI;
    initialize();
}

NodeConnectorUI::~NodeConnectorUI()
{
    connectedTo = nullptr;
    parent = nullptr;
}

void NodeConnectorUI::initialize()
{
    setAlwaysOnTop(true);
    setPosition(0.0, 0.0);
    setBounds(0, 0, NODE_SIZE, NODE_SIZE);
}

void NodeConnectorUI::paint(Graphics& g)
{
    g.setColour(Colour::fromString(NODE_COLOUR));

    double x = getLocalBounds().getX();
    double y = getLocalBounds().getY();
    
    if (type == NodeConnectorType::AudioInput)
    {
        //This is to draw circle in center of bounds
        //becuase it gets clipped otherwise
        x += fabs(NODE_SIZE - NODE_INPUT_SIZE) / 2;
        y += fabs(NODE_SIZE - NODE_INPUT_SIZE) / 2;

        g.drawEllipse(x, y, NODE_INPUT_SIZE, NODE_INPUT_SIZE, NODE_LINE_THICKNESS);
    }
    else
    {
        g.fillEllipse(x, y, NODE_SIZE, NODE_SIZE);
    }
}

void NodeConnectorUI::setPosition(float x, float y)
{
    position.setX(x);
    position.setY(y);
}

void NodeConnectorUI::setPosition(Point<float> newPosition)
{
    position = newPosition;
}

void NodeConnectorUI::mouseDown(const MouseEvent& e)
{
    for (auto listener : listeners)
    {
        if (e.mods.isLeftButtonDown())
            listener->onNodeLeftClick(this, e);
        else if (e.mods.isRightButtonDown())
            listener->onNodeRightClick(this, e);
    }
}

void NodeConnectorUI::mouseDrag(const MouseEvent& e)
{
    for (auto listener : listeners)
    {
        listener->onNodeDrag(this, e);
    }
}

void NodeConnectorUI::mouseUp(const MouseEvent& e)
{
    if (e.mods.isLeftButtonDown())
    {
        for (auto listener : listeners)
        {
            listener->onNodeLeftRelease(this, e);
        }
    }
}

void NodeConnectorUI::setType(NodeConnectorType type)
{
    this->type = type;
}

NodeConnectorType NodeConnectorUI::getType()
{
    return type;
}

Uuid NodeConnectorUI::getId()
{
    return id;
}

void NodeConnectorUI::addListener(NodeConnectorUI::Listener* listener)
{
    listeners.add(listener);
}

NodeUI* NodeConnectorUI::getAttachedNodeUI()
{
    return attachedProcessorUI;
}

