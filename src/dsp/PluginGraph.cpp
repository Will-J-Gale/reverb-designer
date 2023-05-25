#include <dsp/PluginGraph.h>
#include <dsp/AudioProcessorFactory.h>

PluginGraph::~PluginGraph()
{
    if (_pluginState != nullptr)
    {
        _pluginState->deleteAllChildElements();
    }
}

void PluginGraph::reset(double sampleRate)
{
    _sampleRate = sampleRate;
    _inputs.clear();
    _outputs.clear();
    _allNodes.clear();
}

void PluginGraph::process(std::vector<float>& inputFrame, int numInputChannels)
{
    if (_updateProcessorsFlag)
    {
        updateProcessors();
        _updateProcessorsFlag = false;
        return;
    }

    if (_processPathNeedsUpdating)
    {
        calculateProcessPath();
        return;
    }

    if(_clearAllNodes)
    {
        clear();
        return;
    }

    if(_deleteNodes)
    {
        performDelete();
        return;
    }

for (int i = 0; i < _inputs.size(); i++)
    {
        auto input = _inputs[i];
        _inputs[i]->process(inputFrame[i]);
    } 

    for (auto processors : _processPath)
    {
        for (auto processor : processors)
        {
            processor->process();
        }
    }

    for (auto output : _outputs)
    {
        output->process();
    }

    resetProcessors();
}

bool PluginGraph::canProcessAudioFrame()
{
	return false;
}

Array<AudioProcessorNode*> PluginGraph::processBlocks(Array<AudioProcessorNode*> blockToProcess)
{
    Array<AudioProcessorNode*> outputConnections;

    for (auto* block : blockToProcess)
    {
        block->process();

        auto blockOutputs = block->getReadyOutputConnections();

        blockOutputs.removeValuesIn(outputConnections);
        outputConnections.addArray(blockOutputs);
    }

    return outputConnections;
}

void PluginGraph::createDSPObject(NodeInstance dspObjectType)
{
}

void PluginGraph::initialiseDspObject(IAudioProcessor *dspObject)
{
    dspObject->reset(_sampleRate);
}

void PluginGraph::addProcessorNode(AudioProcessorNodePtr processorNode)
{
    _allNodes.add(processorNode);
}

void PluginGraph::deleteProcessorNode(AudioProcessorNode* processorNode)
{
    _deleteNodes = true;
    _nodesToDelete.add(processorNode);
}

void PluginGraph::deleteProcessorNodes(Array<AudioProcessorNode*> processorNodes)
{
    _deleteNodes = true;
    _nodesToDelete.addArray(processorNodes);
}

void PluginGraph::performDelete()
{
    _deleteNodes = false;
    for(AudioProcessorNode* node : _nodesToDelete)
    {
        removeFromArray(_allNodes, node);
    }

    _nodesToDelete.clear();
}

AudioProcessorNodePtr PluginGraph::generateProcessorNode(NodeInstance type)
{
    auto processorNode = std::make_shared<AudioProcessorNode>(type);
    auto audioProcessor = AudioProcessorFactory::Generate(type);

    initialiseDspObject(audioProcessor.get());
    processorNode->setProcessor(audioProcessor);

    _allNodes.add(processorNode);

    return processorNode;
}

bool PluginGraph::isCreatingFeedback(AudioProcessorNode* start, AudioProcessorNode* end)
{
    Array<AudioProcessorNode*> outputConnections;

    outputConnections.addArray(start->getOutputConnections());

    while (outputConnections.size() > 0)
    {
        Array<AudioProcessorNode*> outputs;

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
        _inputs.add(std::make_shared<AudioProcessorNode>());
    }
}

void PluginGraph::addOutputs(int numOutputs)
{
    for (int i = 0; i < numOutputs; i++)
    {
        _outputs.add(std::make_shared<AudioProcessorNode>());
    }
}

Array<AudioProcessorNodePtr> PluginGraph::getInputs()
{
    return _inputs;
}

Array<AudioProcessorNodePtr>PluginGraph::getOutputs()
{
    return _outputs;
}

int PluginGraph::getNumInputs()
{
    return _inputs.size();
}

int PluginGraph::getNumOutputs()
{
    return _inputs.size();
}

double PluginGraph::getOutputSampleFrom(int outputChannel)
{
    if (_outputs.size() - 1 < outputChannel)
        return 0.0;

    return _outputs[outputChannel]->getInputSample();
}

double PluginGraph::getLeftOutputSample()
{
    if (_inputs.size() >= 2)
        return _outputs[0]->getInputSample();

    return 0.0;
}

double PluginGraph::getRightOutputSample()
{
    if (_inputs.size() >= 2)
        return _outputs[0]->getInputSample();

    return 0.0;
}

void PluginGraph::setPluginState(XmlElementPtr pluginState)
{
    _pluginState = pluginState;
}

XmlElementPtr PluginGraph::getPluginState()
{
    return _pluginState;
}

IdToAudioProcessorMap PluginGraph::getAudioProcessorNodeMap()
{
    IdToAudioProcessorMap map;
    
    for (auto node : _inputs)
    {
        auto id = node->getIdAsString();
        map[id] = node;
    }

    for (auto node : _outputs)
    {
        auto id = node->getIdAsString();
        map[id] = node;
    }

    for (auto node : _allNodes)
    {
        auto id = node->getIdAsString();
        map[id] = node;
    }

    return map;
}

Array<AudioProcessorNodePtr> PluginGraph::getAllBlocks()
{
    return _allNodes;
}

void PluginGraph::clear()
{
    for(AudioProcessorNodePtr& input : _inputs)
    {
        input->disconnectAll();
    }

    for(AudioProcessorNodePtr& output : _outputs)
    {
        output->disconnectAll();
    }

    for(AudioProcessorNodePtr& node : _allNodes)
    {
        node->disconnectAll();
    }

    // inputs.clear();
    // outputs.clear();
    _allNodes.clear();
}

void PluginGraph::clearFromUI()
{
    _clearAllNodes = true;
}

void PluginGraph::deleteAndReplaceAudioBlocks(std::function<std::shared_ptr<AudioProcessorState>()> callback)
{
    _updateProcessorsFlag = true;
    _generateCallback = callback;
}

void PluginGraph::deleteAndReplaceAudioBlocks(std::shared_ptr<AudioProcessorState> state)
{
    _updateProcessorsFlag = true;
    _tempProcessorState = state;
}

void PluginGraph::calculateProcessPath()
{
    _processPathNeedsUpdating = false;
    _processPath.clear();

    Array<AudioProcessorNode*> currentlyProcessing;
    Array<AudioProcessorNode*> finishedProcessing;

    for (int i = 0; i < _inputs.size(); i++)
    {
        auto input = _inputs[i];
        _inputs[i]->process(0);
        currentlyProcessing.addArray(_inputs[i]->getReadyOutputConnections());
    }

    _processPath.add(currentlyProcessing);

    while (!currentlyProcessing.isEmpty())
    {
        Array<AudioProcessorNode*> outputConnections;

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
            _processPath.add(currentlyProcessing);
        }  
    }
}

void PluginGraph::updateProcessPath()
{
    _processPathNeedsUpdating = true;
}

void PluginGraph::updateProcessors()
{
    _allNodes.clear();
    _inputs.clear();
    _outputs.clear();

    // auto newProcessors = generateCallback();

    _inputs = _tempProcessorState->inputs;
    _outputs = _tempProcessorState->outputs;
    _allNodes = _tempProcessorState->nodes;

    calculateProcessPath();
}

void PluginGraph::resetProcessors()
{
    for (auto block : _allNodes)
    {
        block->setFinishedProcessing(false);
    }
}
