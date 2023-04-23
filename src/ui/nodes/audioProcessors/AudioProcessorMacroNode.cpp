#include <ui/nodes/audioProcessors/AudioProcessorMacroNode.h>
#include <ui/GraphEditor.h>

AudioProcessorMacroNode::AudioProcessorMacroNode(String name) : AudioProcessorNodeUI(name, NodeInstance::Macro)
{
    setBounds(0, 0, GAIN_WIDTH, GAIN_HEIGHT);

    addInputConnector();
    addOutputConnector();

    nameLabel.setBounds(0, 0, GAIN_WIDTH, TEXT_HEIGHT);
    updateNameAndReCenter("Hello there");

    graphEditor = std::make_shared<GraphEditor>();
}

AudioProcessorMacroNode::~AudioProcessorMacroNode()
{
}

void AudioProcessorMacroNode::mouseDoubleClick(const MouseEvent& e)
{
    auto closeCallback = std::bind(&AudioProcessorMacroNode::closeWindow, this);
    window = std::make_unique<AudioProcessorMacroWindow>("Macro", closeCallback);
    window->setContentNonOwned(graphEditor.get(), true);
}

void AudioProcessorMacroNode::closeWindow()
{
    window.reset();
}

std::string AudioProcessorMacroNode::getIdAsString()
{
    return id.toString().toStdString();
}