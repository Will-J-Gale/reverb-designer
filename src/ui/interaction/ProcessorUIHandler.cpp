/*
  ==============================================================================

    ProcessorUIHandler.cpp
    Created: 26 Oct 2020 9:45:05pm
    Author:  Will

  ==============================================================================
*/

#include <ui/interaction/ProcessorUIHandler.h>
#include <ui/audioProcessorNodes/AudioProcessorNode.h>
#include <dsp/AudioProcessingBlock.h>
#include <ui/audioProcessorNodes/AudioProcessorNodeFactory.h>
#include <ui/GraphEditor.h>
#include <dsp/PluginGraph.h>
#include <ui/interaction/ConnectionHandler.h>

ProcessorUIHandler::ProcessorUIHandler(GraphEditor* graphEditor)
{
    this->graphEditor = graphEditor;
}

AudioProcessorNodePtr ProcessorUIHandler::createDspObject(DspObjectType type, Point<int> position)
{
    if (type == DspObjectType::Input || type == DspObjectType::Output)
        jassert("Cannot create input or output processor without existing providing AudioProcessingBlock");

    auto processorUI = AudioProcessorNodeFactory::Generate(type);
    auto processingBlock = graphEditor->pluginGraph->generateProcessingBlock(type);

    processorUI->setProcessingBlock(processingBlock);
    processorUI->setTopLeftPosition(position);

    return processorUI;
}

AudioProcessorNodePtr ProcessorUIHandler::createDspObject(DspObjectType type, Point<int> position, AudioProcessingBlockPtr processingBlock)
{
    AudioProcessorNodePtr processorUI = AudioProcessorNodeFactory::Generate(type);

    processorUI->setProcessingBlock(processingBlock);
    processorUI->setTopLeftPosition(position);

    return processorUI;
}

void ProcessorUIHandler::initializeProcessor(AudioProcessorNodePtr processor)
{
    processor->addListener(graphEditor);
    graphEditor->processors.add(processor);
    graphEditor->nodes.addArray(processor->getAllNodes());
    graphEditor->addNodeListeners(processor->getAllNodes());
    graphEditor->addAndMakeVisible(processor.get());
}

void ProcessorUIHandler::deleteProcessor(AudioProcessorNode* processor)
{
    Array<Node*> processorNodes = processor->getAllNodes();
    for (auto* node : processorNodes)
    {
        graphEditor->connectionHandler.deleteConnection(node);
        graphEditor->removeFromArray(graphEditor->nodes, node);
    }

    graphEditor->pluginGraph->deleteProcessingBlock(processor->getProcessingBlock());
    graphEditor->removeFromArray(graphEditor->processors, processor);
    graphEditor->pluginGraph->updateProcessPath();
}

void ProcessorUIHandler::duplicateProcessor(AudioProcessorNode* processor)
{
    DspObjectType type = processor->getType();

    auto newProcessor = createDspObject(type, processor->getPosition () + Point<int>(DUPLICATE_OFFSET_X, DUPLICATE_OFFSET_Y));
    auto parameters = processor->getAudioParametersAsXml();
    newProcessor->setAudioParametersFromXml(parameters);
    delete parameters;
    initializeProcessor(newProcessor);
}
