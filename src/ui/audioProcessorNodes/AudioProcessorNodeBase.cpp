/*
  ==============================================================================

    BaseAudioProcessor.cpp
    Created: 10 Sep 2020 6:52:28pm
    Author:  Will

  ==============================================================================
*/

#include <ui/audioProcessorNodes/AudioProcessorNode.h>
#include <ui/GraphEditor.h>
#include <dsp/Delay.h>


AudioProcessorNode::AudioProcessorNode()
{
    initialize();
}

AudioProcessorNode::AudioProcessorNode(DspObjectType type)
{
    this->type = type;
    initialize();
}

AudioProcessorNode::~AudioProcessorNode()
{
}

void AudioProcessorNode::initialize()
{
    setBounds(0, 0, 200, 50);

    nameLabel.setBounds(getBounds());
    nameLabel.setJustificationType(Justification::centredTop);
    nameLabel.setInterceptsMouseClicks(false, false);
    nameLabel.setColour(Label::textColourId, Colour::fromString(TEXT_COLOUR));
    nameLabel.setFont(Font(FONT_SIZE, Font::bold));

    addAndMakeVisible(nameLabel);
}

Point<int> AudioProcessorNode::getCenterPosition()
{
    auto bounds = getBounds();
    Point<int> pos;

    pos.setX(bounds.getX() + (bounds.getWidth() / 2));
    pos.setY(bounds.getY() + (bounds.getHeight() / 2));

    return pos;
}

void AudioProcessorNode::paint(Graphics& g)
{
    Rectangle<int> bounds = getLocalBounds();
    bounds.setX(bounds.getX() + NODE_SIZE);
    bounds.setWidth(bounds.getWidth() - (2 * NODE_SIZE));

    g.setColour(Colour::fromString(AUDIO_PROCESSOR_COLOUR));
    g.drawRoundedRectangle(bounds.toFloat(), AUDIO_PROCESSOR_CORNER_SIZE, AUDIO_PROCESSOR_THICKNESS);
    g.fillRoundedRectangle(bounds.toFloat(), AUDIO_PROCESSOR_CORNER_SIZE);
}

void AudioProcessorNode::mouseDown(const MouseEvent& e)
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

void AudioProcessorNode::mouseDrag(const MouseEvent& e)
{
    dragHandler.drag(e);

    for (auto listener : listeners)
    {
        listener->onProcessorMoved(this, e);
    }
}

void AudioProcessorNode::mouseUp(const MouseEvent& e)
{
    for (auto listener : listeners)
    {
        listener->onProcessorReleased(this, e);
    }
}

void AudioProcessorNode::updateNameAndReCenter(String name)
{
    nameLabel.setText(name, dontSendNotification);
    nameLabel.setSize(getBounds().getWidth(), nameLabel.getBounds().getHeight());
}

Array<Node*> AudioProcessorNode::getAllNodes()
{
    Array<Node*> nodes;

    if (input != nullptr)
        nodes.add(input.get());

    if (output != nullptr)
        nodes.add(output.get());

    return nodes;
}

Node* AudioProcessorNode::getInputNode()
{
    return input.get();
}

Node* AudioProcessorNode::getOutputNode()
{
	return output.get();
}

void AudioProcessorNode::addInputNode()
{
    input = std::make_shared<Node>(this);
    input->setType(NodeType::AudioInput);
    input->setCentrePosition(NODE_SIZE / 2, getHeight() / 2);
    
    addAndMakeVisible(input.get());  
}

void AudioProcessorNode::addOutputNode()
{
    output = std::make_shared<Node>(this);
    output->setType(NodeType::AudioOutput);
    output->setCentrePosition(getWidth() - (NODE_SIZE / 2), getHeight() / 2);
    
    addAndMakeVisible(output.get());
}

void AudioProcessorNode::addListener(AudioProcessorNode::Listener* listener)
{
    listeners.add(listener);
}

AudioProcessingBlockPtr AudioProcessorNode::getProcessingBlock()
{
	return processingBlock;
}

void AudioProcessorNode::handleRightClick()
{
    AudioProcessorConextMenuItems selection = (AudioProcessorConextMenuItems)contextMenu.show();

    for (auto listener : listeners)
    {
        listener->onContextSelection(this, selection);
    }
}

void AudioProcessorNode::setProcessingBlock(AudioProcessingBlockPtr processingBlock)
{
    if (processingBlock->getType() != type)
    {
        jassert("Cannot pass processing block of different type to UI");
    }

    this->processingBlock = processingBlock;
    this->setUIParameters();
}

IAudioProcessor* AudioProcessorNode::getAudioProcessor()
{
    return processingBlock->getProcessor();
}

void AudioProcessorNode::reverse()
{
    reversed = !reversed;

    auto inputPosition = input->getPosition();
    auto outputPosition = output->getPosition();

    inputPosition.addXY(input->getWidth() / 2, input->getHeight() / 2);
    outputPosition.addXY(output->getWidth() / 2, output->getHeight() / 2);

    input->setCentrePosition(outputPosition);
    output->setCentrePosition(inputPosition);
}

std::string AudioProcessorNode::getIdAsString()
{
    return processingBlock->getIdAsString();
}

DspObjectType AudioProcessorNode::getType()
{
    return type;
}

bool AudioProcessorNode::isReversed()
{
    return reversed;
}

void AudioProcessorNode::connectInput(AudioProcessorNode* connection)
{
    if (!inputConnections.contains(connection))
    {
        inputConnections.add(connection);
        this->processingBlock->connectInput(connection->getProcessingBlock().get());
    }
    
}

void AudioProcessorNode::connectFeedbackInput(AudioProcessorNode* connection)
{
    if (!feedbackConnections.contains(connection))
    {
        feedbackConnections.add(connection);
        this->processingBlock->connectFeedbackInput(connection->getProcessingBlock().get());
    }
}

void AudioProcessorNode::connectOutput(AudioProcessorNode* connection)
{
    if (!outputConnections.contains(connection))
    {
        outputConnections.add(connection);
        this->processingBlock->connectOutput(connection->getProcessingBlock().get());
    } 
}

void AudioProcessorNode::disconnectInput(AudioProcessorNode* connection)
{
    removeFromArray(inputConnections, connection);
    this->processingBlock->disconnectInput(connection->getProcessingBlock().get());
}

void AudioProcessorNode::disconnectOutput(AudioProcessorNode* connection)
{
    removeFromArray(outputConnections, connection);
    this->processingBlock->disconnectOutput(connection->getProcessingBlock().get());
}

Array<AudioProcessorNode*> AudioProcessorNode::getOutputConnections()
{
    return outputConnections;
}

Array<AudioProcessorNode*> AudioProcessorNode::getInputConnections()
{
    return inputConnections;
}

Array<AudioProcessorNode*> AudioProcessorNode::getFeedbackConnections()
{
    return feedbackConnections;
}

void AudioProcessorNode::addExistingInput(AudioProcessorNode* inputProcessor)
{
    inputConnections.add(inputProcessor);
}

void AudioProcessorNode::addExistingOutput(AudioProcessorNode* outputProcessor)
{
    outputConnections.add(outputProcessor);
}
