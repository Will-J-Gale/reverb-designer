/*
  ==============================================================================

    BaseAudioProcessor.cpp
    Created: 10 Sep 2020 6:52:28pm
    Author:  Will

  ==============================================================================
*/

#include <ui/audioProcessor/AudioProcessorUI.h>
#include <ui/GraphEditor.h>
#include <dsp/Delay.h>


AudioProcessorUI::AudioProcessorUI()
{
    initialize();
}

AudioProcessorUI::AudioProcessorUI(DspObjectType type)
{
    this->type = type;
    initialize();
}

AudioProcessorUI::~AudioProcessorUI()
{
}

void AudioProcessorUI::initialize()
{
    setBounds(0, 0, 200, 50);

    nameLabel.setBounds(getBounds());
    nameLabel.setJustificationType(Justification::centredTop);
    nameLabel.setInterceptsMouseClicks(false, false);
    nameLabel.setColour(Label::textColourId, Colour::fromString(TEXT_COLOUR));
    nameLabel.setFont(Font(FONT_SIZE, Font::bold));

    addAndMakeVisible(nameLabel);
}

Point<int> AudioProcessorUI::getCenterPosition()
{
    auto bounds = getBounds();
    Point<int> pos;

    pos.setX(bounds.getX() + (bounds.getWidth() / 2));
    pos.setY(bounds.getY() + (bounds.getHeight() / 2));

    return pos;
}

void AudioProcessorUI::paint(Graphics& g)
{
    Rectangle<int> bounds = getLocalBounds();
    bounds.setX(bounds.getX() + NODE_SIZE);
    bounds.setWidth(bounds.getWidth() - (2 * NODE_SIZE));

    g.setColour(Colour::fromString(AUDIO_PROCESSOR_COLOUR));
    g.drawRoundedRectangle(bounds.toFloat(), AUDIO_PROCESSOR_CORNER_SIZE, AUDIO_PROCESSOR_THICKNESS);
    g.fillRoundedRectangle(bounds.toFloat(), AUDIO_PROCESSOR_CORNER_SIZE);
}

void AudioProcessorUI::mouseDown(const MouseEvent& e)
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

void AudioProcessorUI::mouseDrag(const MouseEvent& e)
{
    dragHandler.drag(e);

    for (auto listener : listeners)
    {
        listener->onProcessorMoved(this, e);
    }
}

void AudioProcessorUI::mouseUp(const MouseEvent& e)
{
    for (auto listener : listeners)
    {
        listener->onProcessorReleased(this, e);
    }
}

void AudioProcessorUI::updateNameAndReCenter(String name)
{
    nameLabel.setText(name, dontSendNotification);
    nameLabel.setSize(getBounds().getWidth(), nameLabel.getBounds().getHeight());
}

Array<Node*> AudioProcessorUI::getAllNodes()
{
    Array<Node*> nodes;

    if (input != nullptr)
        nodes.add(input.get());

    if (output != nullptr)
        nodes.add(output.get());

    return nodes;
}

Node* AudioProcessorUI::getInputNode()
{
    return input.get();
}

Node* AudioProcessorUI::getOutputNode()
{
	return output.get();
}

void AudioProcessorUI::addInputNode()
{
    input = std::make_shared<Node>(this);
    input->setType(NodeType::AudioInput);
    input->setCentrePosition(NODE_SIZE / 2, getHeight() / 2);
    
    addAndMakeVisible(input.get());  
}

void AudioProcessorUI::addOutputNode()
{
    output = std::make_shared<Node>(this);
    output->setType(NodeType::AudioOutput);
    output->setCentrePosition(getWidth() - (NODE_SIZE / 2), getHeight() / 2);
    
    addAndMakeVisible(output.get());
}

void AudioProcessorUI::addListener(AudioProcessorUI::Listener* listener)
{
    listeners.add(listener);
}

AudioProcessingBlockPtr AudioProcessorUI::getProcessingBlock()
{
	return processingBlock;
}

void AudioProcessorUI::handleRightClick()
{
    AudioProcessorConextMenuItems selection = (AudioProcessorConextMenuItems)contextMenu.show();

    for (auto listener : listeners)
    {
        listener->onContextSelection(this, selection);
    }
}

void AudioProcessorUI::setProcessingBlock(AudioProcessingBlockPtr processingBlock)
{
    if (processingBlock->getType() != type)
    {
        jassert("Cannot pass processing block of different type to UI");
    }

    this->processingBlock = processingBlock;
    this->setUIParameters();
}

IAudioProcessor* AudioProcessorUI::getAudioProcessor()
{
    return processingBlock->getProcessor();
}

void AudioProcessorUI::reverse()
{
    reversed = !reversed;

    auto inputPosition = input->getPosition();
    auto outputPosition = output->getPosition();

    inputPosition.addXY(input->getWidth() / 2, input->getHeight() / 2);
    outputPosition.addXY(output->getWidth() / 2, output->getHeight() / 2);

    input->setCentrePosition(outputPosition);
    output->setCentrePosition(inputPosition);
}

std::string AudioProcessorUI::getIdAsString()
{
    return processingBlock->getIdAsString();
}

DspObjectType AudioProcessorUI::getType()
{
    return type;
}

bool AudioProcessorUI::isReversed()
{
    return reversed;
}

void AudioProcessorUI::connectInput(AudioProcessorUI* connection)
{
    if (!inputConnections.contains(connection))
    {
        inputConnections.add(connection);
        this->processingBlock->connectInput(connection->getProcessingBlock().get());
    }
    
}

void AudioProcessorUI::connectFeedbackInput(AudioProcessorUI* connection)
{
    if (!feedbackConnections.contains(connection))
    {
        feedbackConnections.add(connection);
        this->processingBlock->connectFeedbackInput(connection->getProcessingBlock().get());
    }
}

void AudioProcessorUI::connectOutput(AudioProcessorUI* connection)
{
    if (!outputConnections.contains(connection))
    {
        outputConnections.add(connection);
        this->processingBlock->connectOutput(connection->getProcessingBlock().get());
    } 
}

void AudioProcessorUI::disconnectInput(AudioProcessorUI* connection)
{
    removeFromArray(inputConnections, connection);
    this->processingBlock->disconnectInput(connection->getProcessingBlock().get());
}

void AudioProcessorUI::disconnectOutput(AudioProcessorUI* connection)
{
    removeFromArray(outputConnections, connection);
    this->processingBlock->disconnectOutput(connection->getProcessingBlock().get());
}

Array<AudioProcessorUI*> AudioProcessorUI::getOutputConnections()
{
    return outputConnections;
}

Array<AudioProcessorUI*> AudioProcessorUI::getInputConnections()
{
    return inputConnections;
}

Array<AudioProcessorUI*> AudioProcessorUI::getFeedbackConnections()
{
    return feedbackConnections;
}

void AudioProcessorUI::addExistingInput(AudioProcessorUI* inputProcessor)
{
    inputConnections.add(inputProcessor);
}

void AudioProcessorUI::addExistingOutput(AudioProcessorUI* outputProcessor)
{
    outputConnections.add(outputProcessor);
}
