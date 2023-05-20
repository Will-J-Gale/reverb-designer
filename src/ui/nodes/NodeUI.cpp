#include <ui/nodes/NodeUI.h>

NodeUI::NodeUI(String name, NodeClass nodeClass, NodeInstance nodeInstance)
{
    this->name = name;
    this->nodeClass = nodeClass;
    this->nodeInstance = nodeInstance;

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
    input->setCentrePosition(CONNECTOR_SIZE / 2, getHeight() / 2);
    
    addAndMakeVisible(input.get());  
}

void NodeUI::addOutputConnector()
{
    output = std::make_shared<NodeConnectorUI>(this);
    output->setType(NodeConnectorType::AudioOutput);
    output->setCentrePosition(getWidth() - (CONNECTOR_SIZE / 2), getHeight() / 2);
    
    addAndMakeVisible(output.get());
}

NodeConnectorUI* NodeUI::getInputConnector()
{
    return input.get();
}

NodeConnectorUI* NodeUI::getOutputConnector()
{
	return output.get();
}

Array<NodeConnectorUI*> NodeUI::getAllNodeConnectors()
{
    Array<NodeConnectorUI*> nodes;

    if (input != nullptr)
        nodes.add(input.get());

    if (output != nullptr)
        nodes.add(output.get());

    return nodes;
}

// void NodeUI::connectInput(NodeUI* connection)
// {
//     if (!inputConnections.contains(connection))
//     {
//         inputConnections.add(connection);
//         // this->processorNode->connectInput(connection->getProcessorNode().get());
//     }
    
// }

// void NodeUI::connectFeedbackInput(NodeUI* connection)
// {
//     if (!feedbackConnections.contains(connection))
//     {
//         feedbackConnections.add(connection);
//         // this->processorNode->connectFeedbackInput(connection->getProcessorNode().get());
//     }
// }

// void NodeUI::connectOutput(NodeUI* connection)
// {
//     if (!outputConnections.contains(connection))
//     {
//         outputConnections.add(connection);
//         // this->processorNode->connectOutput(connection->getProcessorNode().get());
//     } 
// }

// void NodeUI::disconnectInput(NodeUI* connection)
// {
//     removeFromArray(inputConnections, connection);
//     // this->processorNode->disconnectInput(connection->getProcessorNode().get());
// }

// void NodeUI::disconnectOutput(NodeUI* connection)
// {
//     removeFromArray(outputConnections, connection);
//     // this->processorNode->disconnectOutput(connection->getProcessorNode().get());
// }

Array<NodeUI*> NodeUI::getOutputConnections()
{
    return outputConnections;
}

Array<NodeUI*> NodeUI::getInputConnections()
{
    return inputConnections;
}

Array<NodeUI*> NodeUI::getFeedbackConnections()
{
    return feedbackConnections;
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
    dragHandler.mouseDown(e);

    if (e.mods.isRightButtonDown())
    {
        handleRightClick();
    }
    else
    {
        for (auto listener : listeners)
        {
            listener->onNodeClicked(this, e);
        }
    }
}

void NodeUI::mouseDrag(const MouseEvent& e)
{
    dragHandler.drag(e);

    for (auto listener : listeners)
    {
        listener->onNodeMoved(this, e);
    }
}

void NodeUI::mouseUp(const MouseEvent& e)
{
    for (auto listener : listeners)
    {
        listener->onNodeReleased(this, e);
    }
}

void NodeUI::handleRightClick()
{
    NodeContextMenuItems selection = (NodeContextMenuItems)contextMenu.show();

    for (auto listener : listeners)
    {
        listener->onNodeContextSelection(this, selection);
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

void NodeUI::reverse()
{
    reversed = !reversed;

    auto inputPosition = input->getPosition();
    auto outputPosition = output->getPosition();

    inputPosition.addXY(input->getWidth() / 2, input->getHeight() / 2);
    outputPosition.addXY(output->getWidth() / 2, output->getHeight() / 2);

    input->setCentrePosition(outputPosition);
    output->setCentrePosition(inputPosition);
}

bool NodeUI::isReversed()
{
    return reversed;
}

std::string NodeUI::getIdAsString()
{
    return id.toString().toStdString();
}

NodeClass NodeUI::getNodeClass()
{
    return nodeClass;
}

NodeInstance NodeUI::getNodeInstance()
{
    return nodeInstance;
}

String NodeUI::getNodeName()
{
    return name;
}