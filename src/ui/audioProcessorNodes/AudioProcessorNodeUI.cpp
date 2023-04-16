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

AudioProcessorNodeUI::AudioProcessorNodeUI(DspObjectType type)
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

Array<Node*> AudioProcessorNodeUI::getAllNodes()
{
    Array<Node*> nodes;

    if (input != nullptr)
        nodes.add(input.get());

    if (output != nullptr)
        nodes.add(output.get());

    return nodes;
}

Node* AudioProcessorNodeUI::getInputNode()
{
    return input.get();
}

Node* AudioProcessorNodeUI::getOutputNode()
{
	return output.get();
}

void AudioProcessorNodeUI::addInputNode()
{
    input = std::make_shared<Node>(this);
    input->setType(NodeType::AudioInput);
    input->setCentrePosition(NODE_SIZE / 2, getHeight() / 2);
    
    addAndMakeVisible(input.get());  
}

void AudioProcessorNodeUI::addOutputNode()
{
    output = std::make_shared<Node>(this);
    output->setType(NodeType::AudioOutput);
    output->setCentrePosition(getWidth() - (NODE_SIZE / 2), getHeight() / 2);
    
    addAndMakeVisible(output.get());
}

void AudioProcessorNodeUI::addListener(AudioProcessorNodeUI::Listener* listener)
{
    listeners.add(listener);
}

AudioProcessorNodePtr AudioProcessorNodeUI::getProcessingBlock()
{
	return processingBlock;
}

void AudioProcessorNodeUI::handleRightClick()
{
    AudioProcessorConextMenuItems selection = (AudioProcessorConextMenuItems)contextMenu.show();

    for (auto listener : listeners)
    {
        listener->onContextSelection(this, selection);
    }
}

void AudioProcessorNodeUI::setProcessingBlock(AudioProcessorNodePtr processingBlock)
{
    if (processingBlock->getType() != type)
    {
        jassert("Cannot pass processing block of different type to UI");
    }

    this->processingBlock = processingBlock;
    this->setUIParameters();
}

IAudioProcessor* AudioProcessorNodeUI::getAudioProcessor()
{
    return processingBlock->getProcessor();
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
    return processingBlock->getIdAsString();
}

DspObjectType AudioProcessorNodeUI::getType()
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
        this->processingBlock->connectInput(connection->getProcessingBlock().get());
    }
    
}

void AudioProcessorNodeUI::connectFeedbackInput(AudioProcessorNodeUI* connection)
{
    if (!feedbackConnections.contains(connection))
    {
        feedbackConnections.add(connection);
        this->processingBlock->connectFeedbackInput(connection->getProcessingBlock().get());
    }
}

void AudioProcessorNodeUI::connectOutput(AudioProcessorNodeUI* connection)
{
    if (!outputConnections.contains(connection))
    {
        outputConnections.add(connection);
        this->processingBlock->connectOutput(connection->getProcessingBlock().get());
    } 
}

void AudioProcessorNodeUI::disconnectInput(AudioProcessorNodeUI* connection)
{
    removeFromArray(inputConnections, connection);
    this->processingBlock->disconnectInput(connection->getProcessingBlock().get());
}

void AudioProcessorNodeUI::disconnectOutput(AudioProcessorNodeUI* connection)
{
    removeFromArray(outputConnections, connection);
    this->processingBlock->disconnectOutput(connection->getProcessingBlock().get());
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
