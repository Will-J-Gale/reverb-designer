#include <ui/nodes/NodeConnectorUI.h>
#include <utils/Constants.h>
#include <ui/GraphEditor.h>
#include <ui/nodes/NodeUI.h>

NodeConnectorUI::NodeConnectorUI()
{
    initialize();
}

NodeConnectorUI::NodeConnectorUI(NodeUI* attachedNodeUI)
{
    _attachedNodeUI = attachedNodeUI;
    initialize();
}

NodeConnectorUI::~NodeConnectorUI()
{
    _connectedTo = nullptr;
    _parent = nullptr;
}

void NodeConnectorUI::initialize()
{
    setAlwaysOnTop(true);
    setPosition(0.0, 0.0);
    setBounds(0, 0, CONNECTOR_SIZE, CONNECTOR_SIZE);
}

void NodeConnectorUI::paint(Graphics& g)
{
    g.setColour(Colour::fromString(CONNECTOR_COLOUR));

    double x = getLocalBounds().getX();
    double y = getLocalBounds().getY();
    
    if (_nodeConnectorType == NodeConnectorType::AudioInput)
    {
        //This is to draw circle in center of bounds
        //becuase it gets clipped otherwise
        x += fabs(CONNECTOR_SIZE - INPUT_CONNECTOR_SIZE) / 2;
        y += fabs(CONNECTOR_SIZE - INPUT_CONNECTOR_SIZE) / 2;

        g.drawEllipse(x, y, INPUT_CONNECTOR_SIZE, INPUT_CONNECTOR_SIZE, CONNECTOR_LINE_THICKNESS);
    }
    else
    {
        g.fillEllipse(x, y, CONNECTOR_SIZE, CONNECTOR_SIZE);
    }
}

void NodeConnectorUI::setPosition(float x, float y)
{
    _position.setX(x);
    _position.setY(y);
}

void NodeConnectorUI::setPosition(Point<float> newPosition)
{
    _position = newPosition;
}

void NodeConnectorUI::mouseDown(const MouseEvent& e)
{
    for (auto listener : _listeners)
    {
        if (e.mods.isLeftButtonDown())
            listener->onNodeConnectorLeftClick(this, e);
        else if (e.mods.isRightButtonDown())
            listener->onNodeConnectorRightClick(this, e);
    }
}

void NodeConnectorUI::mouseDrag(const MouseEvent& e)
{
    for (auto listener : _listeners)
    {
        listener->onNodeConnectorDrag(this, e);
    }
}

void NodeConnectorUI::mouseUp(const MouseEvent& e)
{
    if (e.mods.isLeftButtonDown())
    {
        for (auto listener : _listeners)
        {
            listener->onNodeConnectorLeftRelease(this, e);
        }
    }
}

void NodeConnectorUI::setType(NodeConnectorType nodeConnectorType)
{
    _nodeConnectorType = nodeConnectorType;
}

NodeConnectorType NodeConnectorUI::getType()
{
    return _nodeConnectorType;
}

Uuid NodeConnectorUI::getId()
{
    return _id;
}

void NodeConnectorUI::addListener(NodeConnectorUI::Listener* listener)
{
    _listeners.add(listener);
}

NodeUI* NodeConnectorUI::getAttachedNodeUI()
{
    return _attachedNodeUI;
}

