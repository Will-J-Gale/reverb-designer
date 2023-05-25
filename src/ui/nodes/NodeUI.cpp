#include <ui/nodes/NodeUI.h>

NodeUI::NodeUI(String name, NodeClass nodeClass, NodeInstance nodeInstance)
{
    _name = name;
    _nodeClass = nodeClass;
    _nodeInstance = nodeInstance;

    initialize();
}

void NodeUI::initialize()
{
    setBounds(0, 0, 200, 50);

    updateNameAndReCenter(_name);
    _nameLabel.setBounds(getBounds());
    _nameLabel.setJustificationType(Justification::centredTop);
    _nameLabel.setInterceptsMouseClicks(false, false);
    _nameLabel.setColour(Label::textColourId, Colour::fromString(TEXT_COLOUR));
    _nameLabel.setFont(Font(FONT_SIZE, Font::bold));

    addAndMakeVisible(_nameLabel);
}

void NodeUI::addInputConnector()
{
    _inputConnector = std::make_shared<NodeConnectorUI>(this);
    _inputConnector->setType(NodeConnectorType::AudioInput);
    _inputConnector->setCentrePosition(CONNECTOR_SIZE / 2, getHeight() / 2);
    
    addAndMakeVisible(_inputConnector.get());  
}

void NodeUI::addOutputConnector()
{
    _outputConnector = std::make_shared<NodeConnectorUI>(this);
    _outputConnector->setType(NodeConnectorType::AudioOutput);
    _outputConnector->setCentrePosition(getWidth() - (CONNECTOR_SIZE / 2), getHeight() / 2);
    
    addAndMakeVisible(_outputConnector.get());
}

NodeConnectorUI* NodeUI::getInputConnector()
{
    return _inputConnector.get();
}

NodeConnectorUI* NodeUI::getOutputConnector()
{
	return _outputConnector.get();
}

Array<NodeConnectorUI*> NodeUI::getAllNodeConnectors()
{
    Array<NodeConnectorUI*> nodes;

    if (_inputConnector != nullptr)
        nodes.add(_inputConnector.get());

    if (_outputConnector != nullptr)
        nodes.add(_outputConnector.get());

    return nodes;
}

Array<NodeUI*> NodeUI::getOutputConnections()
{
    return _outputConnections;
}

Array<NodeUI*> NodeUI::getInputConnections()
{
    return _inputConnections;
}

Array<NodeUI*> NodeUI::getFeedbackConnections()
{
    return _feedbackConnections;
}

Point<int> NodeUI::getCenterPosition()
{
    auto bounds = getBounds();
    Point<int> pos;

    pos.setX(bounds.getX() + (bounds.getWidth() / 2));
    pos.setY(bounds.getY() + (bounds.getHeight() / 2));

    return pos;
}

void NodeUI::paint(Graphics& g)
{
    Rectangle<int> bounds = getLocalBounds();
    bounds.setX(bounds.getX() + CONNECTOR_SIZE);
    bounds.setWidth(bounds.getWidth() - (2 * CONNECTOR_SIZE));

    g.setColour(Colour::fromString(NODE_COLOUR));
    g.drawRoundedRectangle(bounds.toFloat(), AUDIO_PROCESSOR_CORNER_SIZE, NODE_THICKNESS);
    g.fillRoundedRectangle(bounds.toFloat(), AUDIO_PROCESSOR_CORNER_SIZE);
}

void NodeUI::mouseDown(const MouseEvent& e)
{
    _dragHandler.mouseDown(e);

    if (e.mods.isRightButtonDown())
    {
        handleRightClick();
    }
    else
    {
        for (auto listener : _listeners)
        {
            listener->onNodeClicked(this, e);
        }
    }
}

void NodeUI::mouseDrag(const MouseEvent& e)
{
    _dragHandler.drag(e);

    for (auto listener : _listeners)
    {
        listener->onNodeMoved(this, e);
    }
}

void NodeUI::mouseUp(const MouseEvent& e)
{
    for (auto listener : _listeners)
    {
        listener->onNodeReleased(this, e);
    }
}

void NodeUI::handleRightClick()
{
    NodeContextMenuItems selection = (NodeContextMenuItems)_contextMenu.show();

    for (auto listener : _listeners)
    {
        listener->onNodeContextSelection(this, selection);
    }
}

void NodeUI::updateNameAndReCenter(String name)
{
    _nameLabel.setText(name, dontSendNotification);
    _nameLabel.setSize(getBounds().getWidth(), _nameLabel.getBounds().getHeight());
}

void NodeUI::addListener(Listener* listener)
{
    _listeners.add(listener);
}

void NodeUI::reverse()
{
    _reversed = !_reversed;

    auto inputPosition = _inputConnector->getPosition();
    auto outputPosition = _outputConnector->getPosition();

    inputPosition.addXY(_inputConnector->getWidth() / 2, _inputConnector->getHeight() / 2);
    outputPosition.addXY(_outputConnector->getWidth() / 2, _outputConnector->getHeight() / 2);

    _inputConnector->setCentrePosition(outputPosition);
    _outputConnector->setCentrePosition(inputPosition);
}

bool NodeUI::isReversed()
{
    return _reversed;
}

std::string NodeUI::getIdAsString()
{
    return _id.toString().toStdString();
}

NodeClass NodeUI::getNodeClass()
{
    return _nodeClass;
}

NodeInstance NodeUI::getNodeInstance()
{
    return _nodeInstance;
}

String NodeUI::getNodeName()
{
    return _name;
}