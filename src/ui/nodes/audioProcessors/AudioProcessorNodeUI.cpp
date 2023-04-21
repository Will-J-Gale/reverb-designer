/*
  ==============================================================================

    BaseAudioProcessor.cpp
    Created: 10 Sep 2020 6:52:28pm
    Author:  Will

  ==============================================================================
*/

#include <ui/nodes/audioProcessors/AudioProcessorNodeUI.h>
#include <ui/GraphEditor.h>
#include <dsp/Delay.h>


AudioProcessorNodeUI::AudioProcessorNodeUI(String name, NodeInstance nodeInstance) 
    : NodeUI(name, NodeClass::AudioProcessor, nodeInstance)
{
    initialize();
}

AudioProcessorNodeUI::~AudioProcessorNodeUI()
{
}

AudioProcessorNodePtr AudioProcessorNodeUI::getProcessorNode()
{
	return processorNode;
}

void AudioProcessorNodeUI::setProcessorNode(AudioProcessorNodePtr processorNode)
{
    // jassert(processorNode->getNodeInstance() == nodeInstance);

    this->processorNode = processorNode;
    this->setUIParameters();
}

IAudioProcessor* AudioProcessorNodeUI::getAudioProcessor()
{
    return processorNode->getProcessor();
}



std::string AudioProcessorNodeUI::getIdAsString()
{
    return processorNode->getIdAsString();
}