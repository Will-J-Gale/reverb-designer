#include <ui/nodes/audioProcessors/AudioProcessorMacroNode.h>

AudioProcessorMacroNode::AudioProcessorMacroNode(AudioProcessorNodeType type) : AudioProcessorNodeUI(type)
{
    setBounds(0, 0, GAIN_WIDTH, GAIN_HEIGHT);

    addInputNode();
    addOutputNode();

    nameLabel.setBounds(0, 0, GAIN_WIDTH, TEXT_HEIGHT);
    updateNameAndReCenter("Hello there");
}

AudioProcessorMacroNode::~AudioProcessorMacroNode()
{
}

void AudioProcessorMacroNode::mouseDoubleClick(const MouseEvent& e)
{
    auto closeCallback = std::bind(&AudioProcessorMacroNode::closeWindow, this);
    window = std::make_unique<AudioProcessorMacroWindow>("Macro", closeCallback);
}

void AudioProcessorMacroNode::closeWindow()
{
    window.reset();
}

std::string AudioProcessorMacroNode::getIdAsString()
{
    return id.toString().toStdString();
}