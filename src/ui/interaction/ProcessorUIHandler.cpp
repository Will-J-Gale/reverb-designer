/*
  ==============================================================================

    ProcessorUIHandler.cpp
    Created: 26 Oct 2020 9:45:05pm
    Author:  Will

  ==============================================================================
*/

#include <ui/interaction/ProcessorUIHandler.h>
#include <ui/audioProcessorNodes/AudioProcessorNodeUI.h>
#include <dsp/AudioProcessorNode.h>
#include <ui/audioProcessorNodes/AudioProcessorNodeUIFactory.h>
#include <ui/GraphEditor.h>
#include <dsp/PluginGraph.h>
#include <ui/interaction/ConnectionHandler.h>

ProcessorUIHandler::ProcessorUIHandler(GraphEditor* graphEditor)
{
    this->graphEditor = graphEditor;
}

AudioProcessorNodeUIPtr ProcessorUIHandler::createDspObject(DspObjectType type, Point<int> position)
{
    if (type == DspObjectType::Input || type == DspObjectType::Output)
        jassert("Cannot create input or output processor without providing AudioProcessorNode");

    auto processorUI = AudioProcessorNodeUIFactory::Generate(type);
    auto processorNode = graphEditor->pluginGraph->generateProcessorNode(type);

    processorUI->setProcessorNode(processorNode);
    processorUI->setTopLeftPosition(position);

    return processorUI;
}

AudioProcessorNodeUIPtr ProcessorUIHandler::createDspObject(DspObjectType type, Point<int> position, AudioProcessorNodePtr processorNode)
{
    AudioProcessorNodeUIPtr processorUI = AudioProcessorNodeUIFactory::Generate(type);

    processorUI->setProcessorNode(processorNode);
    processorUI->setTopLeftPosition(position);

    return processorUI;
}

void ProcessorUIHandler::initializeProcessor(AudioProcessorNodeUIPtr processor)
{
    processor->addListener(graphEditor);
    graphEditor->processors.add(processor);
    graphEditor->nodes.addArray(processor->getAllNodes());
    graphEditor->addNodeListeners(processor->getAllNodes());
    graphEditor->addAndMakeVisible(processor.get());
}

void ProcessorUIHandler::deleteProcessor(AudioProcessorNodeUI* processor)
{
    Array<Node*> processorNodes = processor->getAllNodes();
    for (auto* node : processorNodes)
    {
        graphEditor->connectionHandler.deleteConnection(node);
        graphEditor->removeFromArray(graphEditor->nodes, node);
    }

    graphEditor->pluginGraph->deleteProcessorNode(processor->getProcessorNode());
    graphEditor->removeFromArray(graphEditor->processors, processor);
    graphEditor->pluginGraph->updateProcessPath();
}

void ProcessorUIHandler::duplicateProcessor(AudioProcessorNodeUI* processor)
{
    DspObjectType type = processor->getType();

    auto newProcessor = createDspObject(type, processor->getPosition () + Point<int>(DUPLICATE_OFFSET_X, DUPLICATE_OFFSET_Y));
    auto parameters = processor->getAudioParametersAsXml();
    newProcessor->setAudioParametersFromXml(parameters);
    delete parameters;
    initializeProcessor(newProcessor);
}
