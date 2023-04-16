/*
  ==============================================================================

    Node.cpp
    Created: 10 Sep 2020 6:53:28pm
    Author:  Will

  ==============================================================================
*/

#include <ui/nodes/audioProcessors/AudioProcessorNodeConnectorUI.h>
#include <utils/Constants.h>
#include <ui/GraphEditor.h>
#include <ui/nodes/audioProcessors/AudioProcessorNodeUI.h>

AudioProcessorNodeConnectorUI::AudioProcessorNodeConnectorUI()
{
    initialize();
}

AudioProcessorNodeConnectorUI::AudioProcessorNodeConnectorUI(AudioProcessorNodeUI* attachedProcessorUI)
{
    this->attachedProcessorUI = attachedProcessorUI;
    initialize();
}

AudioProcessorNodeConnectorUI::~AudioProcessorNodeConnectorUI()
{
    connectedTo = nullptr;
    parent = nullptr;
}

void AudioProcessorNodeConnectorUI::initialize()
{
    setAlwaysOnTop(true);
    setPosition(0.0, 0.0);
    setBounds(0, 0, NODE_SIZE, NODE_SIZE);
}

void AudioProcessorNodeConnectorUI::paint(Graphics& g)
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

void AudioProcessorNodeConnectorUI::setPosition(float x, float y)
{
    position.setX(x);
    position.setY(y);
}

void AudioProcessorNodeConnectorUI::setPosition(Point<float> newPosition)
{
    position = newPosition;
}

void AudioProcessorNodeConnectorUI::mouseDown(const MouseEvent& e)
{
    for (auto listener : listeners)
    {
        if (e.mods.isLeftButtonDown())
            listener->onNodeLeftClick(this, e);
        else if (e.mods.isRightButtonDown())
            listener->onNodeRightClick(this, e);
    }
}

void AudioProcessorNodeConnectorUI::mouseDrag(const MouseEvent& e)
{
    for (auto listener : listeners)
    {
        listener->onNodeDrag(this, e);
    }
}

void AudioProcessorNodeConnectorUI::mouseUp(const MouseEvent& e)
{
    if (e.mods.isLeftButtonDown())
    {
        for (auto listener : listeners)
        {
            listener->onNodeLeftRelease(this, e);
        }
    }
}

void AudioProcessorNodeConnectorUI::setType(NodeType type)
{
    this->type = type;
}

NodeType AudioProcessorNodeConnectorUI::getType()
{
    return type;
}

Uuid AudioProcessorNodeConnectorUI::getId()
{
    return id;
}

void AudioProcessorNodeConnectorUI::addListener(AudioProcessorNodeConnectorUI::Listener* listener)
{
    listeners.add(listener);
}

AudioProcessorNodeUI* AudioProcessorNodeConnectorUI::getAttachedProcessorUI()
{
    return attachedProcessorUI;
}

