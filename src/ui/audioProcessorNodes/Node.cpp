/*
  ==============================================================================

    Node.cpp
    Created: 10 Sep 2020 6:53:28pm
    Author:  Will

  ==============================================================================
*/

#include <ui/audioProcessorNodes/Node.h>
#include <utils/Constants.h>
#include <ui/GraphEditor.h>
#include <ui/audioProcessorNodes/AudioProcessorNode.h>

Node::Node()
{
    initialize();
}

Node::Node(AudioProcessorNode* attachedProcessorUI)
{
    this->attachedProcessorUI = attachedProcessorUI;
    initialize();
}

Node::~Node()
{
    connectedTo = nullptr;
    parent = nullptr;
}

void Node::initialize()
{
    setAlwaysOnTop(true);
    setPosition(0.0, 0.0);
    setBounds(0, 0, NODE_SIZE, NODE_SIZE);
}

void Node::paint(Graphics& g)
{
    g.setColour(Colour::fromString(NODE_COLOUR));

    double x = getLocalBounds().getX();
    double y = getLocalBounds().getY();
    
    if (type == NodeType::AudioInput)
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

void Node::setPosition(float x, float y)
{
    position.setX(x);
    position.setY(y);
}

void Node::setPosition(Point<float> newPosition)
{
    position = newPosition;
}

void Node::mouseDown(const MouseEvent& e)
{
    for (auto listener : listeners)
    {
        if (e.mods.isLeftButtonDown())
            listener->onNodeLeftClick(this, e);
        else if (e.mods.isRightButtonDown())
            listener->onNodeRightClick(this, e);
    }
}

void Node::mouseDrag(const MouseEvent& e)
{
    for (auto listener : listeners)
    {
        listener->onNodeDrag(this, e);
    }
}

void Node::mouseUp(const MouseEvent& e)
{
    if (e.mods.isLeftButtonDown())
    {
        for (auto listener : listeners)
        {
            listener->onNodeLeftRelease(this, e);
        }
    }
}

void Node::setType(NodeType type)
{
    this->type = type;
}

NodeType Node::getType()
{
    return type;
}

Uuid Node::getId()
{
    return id;
}

void Node::addListener(Node::Listener* listener)
{
    listeners.add(listener);
}

AudioProcessorNode* Node::getAttachedProcessor()
{
    return attachedProcessorUI;
}

