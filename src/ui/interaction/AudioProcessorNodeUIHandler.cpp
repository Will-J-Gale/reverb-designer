/*
  ==============================================================================

    AudioProcessorNodeUIHandler.cpp
    Created: 26 Oct 2020 9:45:05pm
    Author:  Will

  ==============================================================================
*/

#include <ui/interaction/AudioProcessorNodeUIHandler.h>
#include <ui/audioProcessorNodes/AudioProcessorNodeUI.h>
#include <dsp/AudioProcessorNode.h>
#include <ui/audioProcessorNodes/AudioProcessorNodeUIFactory.h>
#include <ui/GraphEditor.h>
#include <dsp/PluginGraph.h>
#include <ui/interaction/ConnectionHandler.h>

void AudioProcessorNodeUIHandler::initialize(GraphEditor* graphEditor, PluginGraph* pluginGraph)
{
    this->graphEditor = graphEditor;
    this->pluginGraph = pluginGraph;
}

AudioProcessorNodeUIPtr AudioProcessorNodeUIHandler::createAudioProcessorNode(AudioProcessorNodeType type, Point<int> position)
{
    if (type == AudioProcessorNodeType::Input || type == AudioProcessorNodeType::Output)
        jassert("Cannot create input or output processor without providing AudioProcessorNode");

    auto processorNode = pluginGraph->generateProcessorNode(type);
    return createAudioProcessorNode(type, position, processorNode);
}

AudioProcessorNodeUIPtr AudioProcessorNodeUIHandler::createAudioProcessorNode(AudioProcessorNodeType type, Point<int> position, AudioProcessorNodePtr processorNode)
{
    AudioProcessorNodeUIPtr processorNodeUI = AudioProcessorNodeUIFactory::Generate(type);

    processorNodeUI->setProcessorNode(processorNode);
    processorNodeUI->setTopLeftPosition(position);

    return processorNodeUI;
}

AudioProcessorNodeUIPtr AudioProcessorNodeUIHandler::createAudioProcessorNodeUI(AudioProcessorNodeType type, Point<int> position)
{
    AudioProcessorNodeUIPtr processorNodeUI = AudioProcessorNodeUIFactory::Generate(type);
    processorNodeUI->setTopLeftPosition(position);

    return processorNodeUI;
}

void AudioProcessorNodeUIHandler::initializeProcessor(AudioProcessorNodeUIPtr processorNodeUI)
{
    processorNodeUI->addListener(graphEditor);
    graphEditor->processors.add(processorNodeUI);
    graphEditor->nodes.addArray(processorNodeUI->getAllNodes());
    graphEditor->addNodeListeners(processorNodeUI->getAllNodes());
    graphEditor->addAndMakeVisible(processorNodeUI.get());
}

void AudioProcessorNodeUIHandler::deleteProcessor(AudioProcessorNodeUI* processorNodeUI)
{
    Array<AudioProcessorNodeConnectorUI*> processorNodes = processorNodeUI->getAllNodes();
    for (auto* node : processorNodes)
    {
        graphEditor->connectionHandler.deleteConnection(node);
        graphEditor->removeFromArray(graphEditor->nodes, node);
    }

    pluginGraph->deleteProcessorNode(processorNodeUI->getProcessorNode());
    graphEditor->removeFromArray(graphEditor->processors, processorNodeUI);
    pluginGraph->updateProcessPath();
}

void AudioProcessorNodeUIHandler::duplicateProcessor(AudioProcessorNodeUI* processorNodeUI)
{
    AudioProcessorNodeType type = processorNodeUI->getType();

    auto newProcessor = createAudioProcessorNode(type, processorNodeUI->getPosition () + Point<int>(DUPLICATE_OFFSET_X, DUPLICATE_OFFSET_Y));
    auto parameters = processorNodeUI->getAudioParametersAsXml();
    newProcessor->setAudioParametersFromXml(parameters);
    delete parameters;
    initializeProcessor(newProcessor);
}
