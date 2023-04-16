/*
  ==============================================================================

    BaseAudioProcessor.cpp
    Created: 10 Sep 2020 6:52:28pm
    Author:  Will

  ==============================================================================
*/

#include <ui/audioProcessorNodes/AudioProcessorNodeUI.h>
#include <ui/GraphEditor.h>
#include <dsp/Delay.h>


AudioProcessorNodeUI::AudioProcessorNodeUI()
{
    initialize();
}

AudioProcessorNodeUI::AudioProcessorNodeUI(AudioProcessorNodeType type)
{
    this->type = type;
    initialize();
}

AudioProcessorNodeUI::~AudioProcessorNodeUI()
{
}

void AudioProcessorNodeUI::initialize()
{
    setBounds(0, 0, 200, 50);

    nameLabel.setBounds(getBounds());
    nameLabel.setJustificationType(Justification::centredTop);
    nameLabel.setInterceptsMouseClicks(false, false);
    nameLabel.setColour(Label::textColourId, Colour::fromString(TEXT_COLOUR));
    nameLabel.setFont(Font(FONT_SIZE, Font::bold));

    addAndMakeVisible(nameLabel);
}

Point<int> AudioProcessorNodeUI::getCenterPosition()
{
    auto bounds = getBounds();
    Point<int> pos;

    pos.setX(bounds.getX() + (bounds.getWidth() / 2));
    pos.setY(bounds.getY() + (bounds.getHeight() / 2));

    return pos;
}

void AudioProcessorNodeUI::paint(Graphics& g)
{
    Rectangle<int> bounds = getLocalBounds();
    bounds.setX(bounds.getX() + NODE_SIZE);
    bounds.setWidth(bounds.getWidth() - (2 * NODE_SIZE));

    g.setColour(Colour::fromString(AUDIO_PROCESSOR_COLOUR));
    g.drawRoundedRectangle(bounds.toFloat(), AUDIO_PROCESSOR_CORNER_SIZE, AUDIO_PROCESSOR_THICKNESS);
    g.fillRoundedRectangle(bounds.toFloat(), AUDIO_PROCESSOR_CORNER_SIZE);
}

void AudioProcessorNodeUI::mouseDown(const MouseEvent& e)
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

void AudioProcessorNodeUI::mouseDrag(const MouseEvent& e)
{
    dragHandler.drag(e);

    for (auto listener : listeners)
    {
        listener->onProcessorMoved(this, e);
    }
}

void AudioProcessorNodeUI::mouseUp(const MouseEvent& e)
{
    for (auto listener : listeners)
    {
        listener->onProcessorReleased(this, e);
    }
}

void AudioProcessorNodeUI::updateNameAndReCenter(String name)
{
    nameLabel.setText(name, dontSendNotification);
    nameLabel.setSize(getBounds().getWidth(), nameLabel.getBounds().getHeight());
}

Array<AudioProcessorNodeConnectorUI*> AudioProcessorNodeUI::getAllNodes()
{
    Array<AudioProcessorNodeConnectorUI*> nodes;

    if (input != nullptr)
        nodes.add(input.get());

    if (output != nullptr)
        nodes.add(output.get());

    return nodes;
}

AudioProcessorNodeConnectorUI* AudioProcessorNodeUI::getInputNode()
{
    return input.get();
}

AudioProcessorNodeConnectorUI* AudioProcessorNodeUI::getOutputNode()
{
	return output.get();
}

void AudioProcessorNodeUI::addInputNode()
{
    input = std::make_shared<AudioProcessorNodeConnectorUI>(this);
    input->setType(NodeType::AudioInput);
    input->setCentrePosition(NODE_SIZE / 2, getHeight() / 2);
    
    addAndMakeVisible(input.get());  
}

void AudioProcessorNodeUI::addOutputNode()
{
    output = std::make_shared<AudioProcessorNodeConnectorUI>(this);
    output->setType(NodeType::AudioOutput);
    output->setCentrePosition(getWidth() - (NODE_SIZE / 2), getHeight() / 2);
    
    addAndMakeVisible(output.get());
}

void AudioProcessorNodeUI::addListener(AudioProcessorNodeUI::Listener* listener)
{
    listeners.add(listener);
}

AudioProcessorNodePtr AudioProcessorNodeUI::getProcessorNode()
{
	return processorNode;
}

void AudioProcessorNodeUI::handleRightClick()
{
    NodeUIConextMenuItems selection = (NodeUIConextMenuItems)contextMenu.show();

    for (auto listener : listeners)
    {
        listener->onContextSelection(this, selection);
    }
}

void AudioProcessorNodeUI::setProcessorNode(AudioProcessorNodePtr processorNode)
{
    if (processorNode->getType() != type)
    {
        jassert("Cannot pass processing block of different type to UI");
    }

    this->processorNode = processorNode;
    this->setUIParameters();
}

IAudioProcessor* AudioProcessorNodeUI::getAudioProcessor()
{
    return processorNode->getProcessor();
}

void AudioProcessorNodeUI::reverse()
{
    reversed = !reversed;

    auto inputPosition = input->getPosition();
    auto outputPosition = output->getPosition();

    inputPosition.addXY(input->getWidth() / 2, input->getHeight() / 2);
    outputPosition.addXY(output->getWidth() / 2, output->getHeight() / 2);

    input->setCentrePosition(outputPosition);
    output->setCentrePosition(inputPosition);
}

std::string AudioProcessorNodeUI::getIdAsString()
{
    return processorNode->getIdAsString();
}

AudioProcessorNodeType AudioProcessorNodeUI::getType()
{
    return type;
}

bool AudioProcessorNodeUI::isReversed()
{
    return reversed;
}

void AudioProcessorNodeUI::connectInput(AudioProcessorNodeUI* connection)
{
    if (!inputConnections.contains(connection))
    {
        inputConnections.add(connection);
        this->processorNode->connectInput(connection->getProcessorNode().get());
    }
    
}

void AudioProcessorNodeUI::connectFeedbackInput(AudioProcessorNodeUI* connection)
{
    if (!feedbackConnections.contains(connection))
    {
        feedbackConnections.add(connection);
        this->processorNode->connectFeedbackInput(connection->getProcessorNode().get());
    }
}

void AudioProcessorNodeUI::connectOutput(AudioProcessorNodeUI* connection)
{
    if (!outputConnections.contains(connection))
    {
        outputConnections.add(connection);
        this->processorNode->connectOutput(connection->getProcessorNode().get());
    } 
}

void AudioProcessorNodeUI::disconnectInput(AudioProcessorNodeUI* connection)
{
    removeFromArray(inputConnections, connection);
    this->processorNode->disconnectInput(connection->getProcessorNode().get());
}

void AudioProcessorNodeUI::disconnectOutput(AudioProcessorNodeUI* connection)
{
    removeFromArray(outputConnections, connection);
    this->processorNode->disconnectOutput(connection->getProcessorNode().get());
}

Array<AudioProcessorNodeUI*> AudioProcessorNodeUI::getOutputConnections()
{
    return outputConnections;
}

Array<AudioProcessorNodeUI*> AudioProcessorNodeUI::getInputConnections()
{
    return inputConnections;
}

Array<AudioProcessorNodeUI*> AudioProcessorNodeUI::getFeedbackConnections()
{
    return feedbackConnections;
}

void AudioProcessorNodeUI::addExistingInput(AudioProcessorNodeUI* inputProcessor)
{
    inputConnections.add(inputProcessor);
}

void AudioProcessorNodeUI::addExistingOutput(AudioProcessorNodeUI* outputProcessor)
{
    outputConnections.add(outputProcessor);
}
