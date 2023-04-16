#include <ui/nodes/NodeUI.h>

NodeUI::NodeUI(String name, NodeUIType type)
{
    this->type = type;
    this->name = name;

    initialize();
}

void NodeUI::initialize()
{
    setBounds(0, 0, 200, 50);

    updateNameAndReCenter(name);
    nameLabel.setBounds(getBounds());
    nameLabel.setJustificationType(Justification::centredTop);
    nameLabel.setInterceptsMouseClicks(false, false);
    nameLabel.setColour(Label::textColourId, Colour::fromString(TEXT_COLOUR));
    nameLabel.setFont(Font(FONT_SIZE, Font::bold));

    addAndMakeVisible(nameLabel);
}

void NodeUI::addInputConnector()
{
    input = std::make_shared<NodeConnectorUI>(this);
    input->setType(NodeConnectorType::AudioInput);
    input->setCentrePosition(NODE_SIZE / 2, getHeight() / 2);
    
    addAndMakeVisible(input.get());  
}

void NodeUI::addOutputConnector()
{
    output = std::make_shared<NodeConnectorUI>(this);
    output->setType(NodeConnectorType::AudioOutput);
    output->setCentrePosition(getWidth() - (NODE_SIZE / 2), getHeight() / 2);
    
    addAndMakeVisible(output.get());
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
    bounds.setX(bounds.getX() + NODE_SIZE);
    bounds.setWidth(bounds.getWidth() - (2 * NODE_SIZE));

    g.setColour(Colour::fromString(AUDIO_PROCESSOR_COLOUR));
    g.drawRoundedRectangle(bounds.toFloat(), AUDIO_PROCESSOR_CORNER_SIZE, AUDIO_PROCESSOR_THICKNESS);
    g.fillRoundedRectangle(bounds.toFloat(), AUDIO_PROCESSOR_CORNER_SIZE);
}

void NodeUI::mouseDown(const MouseEvent& e)
{
    dragHandler.mouseDown(e);

    if (e.mods.isRightButtonDown())
    {
        handleRightClick();
    }
    else
    {
        for (auto listener : listeners)
        {
            listener->onProcessorClicked(this, e);
        }
    }
}

void NodeUI::mouseDrag(const MouseEvent& e)
{
    dragHandler.drag(e);

    for (auto listener : listeners)
    {
        listener->onProcessorMoved(this, e);
    }
}

void NodeUI::mouseUp(const MouseEvent& e)
{
    for (auto listener : listeners)
    {
        listener->onProcessorReleased(this, e);
    }
}

void NodeUI::handleRightClick()
{
    NodeUIConextMenuItems selection = (NodeUIConextMenuItems)contextMenu.show();

    for (auto listener : listeners)
    {
        listener->onContextSelection(this, selection);
    }
}

void NodeUI::updateNameAndReCenter(String name)
{
    nameLabel.setText(name, dontSendNotification);
    nameLabel.setSize(getBounds().getWidth(), nameLabel.getBounds().getHeight());
}

void NodeUI::addListener(Listener* listener)
{
    listeners.add(listener);
}

bool NodeUI::isReversed()
{
    return reversed;
}

std::string NodeUI::getIdAsString()
{
    return id.toString().toStdString();
}