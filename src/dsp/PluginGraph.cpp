/*
  ==============================================================================

    PluginGraph.cpp
    Created: 19 Sep 2020 1:15:06pm
    Author:  Will

  ==============================================================================
*/

#include <dsp/PluginGraph.h>
#include <dsp/AudioProcessorFactory.h>

PluginGraph::~PluginGraph()
{
    if (pluginState != nullptr)
    {
        pluginState->deleteAllChildElements();
    }
}

bool PluginGraph::reset(double sampleRate)
{
    this->sampleRate = sampleRate;

    inputs.clear();
    outputs.clear();
    allBlocks.clear();

    return true;
}

void PluginGraph::process(std::vector<float>& inputFrame, size_t numInputChannels)
{
    if (updateProcessorsFlag)
    {
        updateProcessors();
        updateProcessorsFlag = false;
        return;
    }

    if (processPathNeedsUpdating)
    {
        calculateProcessPath();
        return;
    }

    for (size_t i = 0; i < inputs.size(); i++)
    {
        auto input = inputs[i];
        inputs[i]->process(inputFrame[i]);
    } 

    for (auto processors : processPath)
    {
        for (auto processor : processors)
        {
            processor->process();
        }
    }

    for (auto output : outputs)
    {
        output->process();
    }

    resetProcessors();
}

bool PluginGraph::canProcessAudioFrame()
{
	return false;
}

Array<AudioProcessingBlock*> PluginGraph::processBlocks(Array<AudioProcessingBlock*> blockToProcess)
{
    Array<AudioProcessingBlock*> outputConnections;

    for (auto* block : blockToProcess)
    {
        block->process();

        auto blockOutputs = block->getReadyOutputConnections();

        blockOutputs.removeValuesIn(outputConnections);
        outputConnections.addArray(blockOutputs);
    }

    return outputConnections;
}

void PluginGraph::createDSPObject(DspObjectType dspObjectType)
{
}


void PluginGraph::initialiseDspObject(IAudioProcessor *dspObject)
{
    if (dspObject == nullptr)
        return;

    if (auto* delayProcessor = dynamic_cast<IDelayProcessor*>(dspObject))
    {
        delayProcessor->reset(sampleRate, 1.0);
    }
    else
    {
        dspObject->reset(sampleRate);
    }
}

void PluginGraph::addProcessingBlock(AudioProcessingBlockPtr processingBlock)
{
    allBlocks.add(processingBlock);
}

void PluginGraph::deleteProcessingBlock(AudioProcessingBlockPtr processingBlock)
{
    removeFromArray(allBlocks, processingBlock);
}

AudioProcessingBlockPtr PluginGraph::generateProcessingBlock(DspObjectType type)
{
    auto audioProcessor = AudioProcessorFactory::Generate(type);
    auto processingBlock = std::make_shared<AudioProcessingBlock>(type);

    initialiseDspObject(audioProcessor.get());
    processingBlock->setProcessor(audioProcessor);

    allBlocks.add(processingBlock);

    return processingBlock;
}

bool PluginGraph::isCreatingFeedback(AudioProcessingBlock* start, AudioProcessingBlock* end)
{
    Array<AudioProcessingBlock*> outputConnections;

    outputConnections.addArray(start->getOutputConnections());

    while (outputConnections.size() > 0)
    {
        Array<AudioProcessingBlock*> outputs;

        for (auto output : outputConnections)
        {
            if (output == end)
            {
                return true;
            }

            outputs.addArray(output->getOutputConnections());
        }

        outputConnections.clear();
        
        if (outputs.size() > 0)
        {
            outputConnections.addArray(outputs);
        }
    }

    return false;
}

void PluginGraph::addInputs(int numInputs)
{
    for (int i = 0; i < numInputs; i++)
    {
        inputs.add(std::make_shared<AudioProcessingBlock>());
    }
}

void PluginGraph::addOutputs(int numOutputs)
{
    for (int i = 0; i < numOutputs; i++)
    {
        outputs.add(std::make_shared<AudioProcessingBlock>());
    }
}

Array<AudioProcessingBlockPtr> PluginGraph::getInputs()
{
    return inputs;
}

Array<AudioProcessingBlockPtr>PluginGraph::getOutputs()
{
    return outputs;
}

int PluginGraph::getNumInputs()
{
    return inputs.size();
}

int PluginGraph::getNumOutputs()
{
    return inputs.size();
}

double PluginGraph::getOutputSampleFrom(int outputChannel)
{
    if (outputs.size() - 1 < outputChannel)
        return 0.0;

    return outputs[outputChannel]->getInputSample();
}

double PluginGraph::getLeftOutputSample()
{
    if (inputs.size() >= 2)
        return outputs[0]->getInputSample();

    return 0.0;
}

double PluginGraph::getRightOutputSample()
{
    if (inputs.size() >= 2)
        return outputs[0]->getInputSample();

    return 0.0;
}

void PluginGraph::setPluginState(XmlElementPtr pluginState)
{
    this->pluginState = pluginState;
}

XmlElementPtr PluginGraph::getPluginState()
{
    return pluginState;
}

std::map<std::string, AudioProcessingBlockPtr> PluginGraph::getAudioProcessingBlockMap()
{
    auto map = std::map<std::string, AudioProcessingBlockPtr>();
    
    for (auto block : inputs)
    {
        auto id = block->getIdAsString();
        map[id] = block;
    }

    for (auto block : outputs)
    {
        auto id = block->getIdAsString();
        map[id] = block;
    }

    for (auto block : allBlocks)
    {
        auto id = block->getIdAsString();
        map[id] = block;
    }

    return map;
}

Array<AudioProcessingBlockPtr> PluginGraph::getAllBlocks()
{
    return allBlocks;
}

void PluginGraph::clear()
{
    pluginState = nullptr;

    inputs.clear();
    outputs.clear();
    allBlocks.clear();
}

void PluginGraph::deleteAndReplaceAudioBlocks(std::function<std::shared_ptr<AudioProcessorState>()> callback)
{
    updateProcessorsFlag = true;
    generateCallback = callback;
}

void PluginGraph::deleteAndReplaceAudioBlocks(std::shared_ptr<AudioProcessorState> state)
{
    updateProcessorsFlag = true;
    tempProcessorState = state;
}

void PluginGraph::calculateProcessPath()
{
    processPathNeedsUpdating = false;
    processPath.clear();

    Array<AudioProcessingBlock*> currentlyProcessing;
    Array<AudioProcessingBlock*> finishedProcessing;

    for (int i = 0; i < inputs.size(); i++)
    {
        auto input = inputs[i];
        inputs[i]->process(0);
        currentlyProcessing.addArray(inputs[i]->getReadyOutputConnections());
    }

    processPath.add(currentlyProcessing);

    while (!currentlyProcessing.isEmpty())
    {
        Array<AudioProcessingBlock*> outputConnections;

        for (auto* block : currentlyProcessing)
        {
            block->process();

            auto blockOutputs = block->getReadyOutputConnections();
            blockOutputs.removeValuesIn(outputConnections);
            outputConnections.addArray(blockOutputs);
        }

        finishedProcessing.addArray(currentlyProcessing);

        currentlyProcessing.clear();

        if (outputConnections.size() > 0)
        {
            currentlyProcessing.addArray(outputConnections);
            processPath.add(currentlyProcessing);
        }  
    }
}

void PluginGraph::updateProcessPath()
{
    processPathNeedsUpdating = true;
}

void PluginGraph::updateProcessors()
{
    allBlocks.clear();
    inputs.clear();
    outputs.clear();

    //auto newProcessors = generateCallback();

    inputs = tempProcessorState->inputs;
    outputs = tempProcessorState->outputs;
    allBlocks = tempProcessorState->allBlocks;

    calculateProcessPath();
}

void PluginGraph::resetProcessors()
{
    for (auto block : allBlocks)
    {
        block->setFinishedProcessing(false);
    }
}
