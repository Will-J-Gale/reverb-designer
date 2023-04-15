/*
  ==============================================================================

    PluginGraph.h
    Created: 19 Sep 2020 1:15:06pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include "GraphEditor.h"
#include "IAudioProcessor.h"
#include "AudioProcessingBlock.h"
#include "ManagedArray.h"
#include "AudioProcessorState.h"
#include "XmlUtils.h"

class PluginGraph : public ManagedArray
{
public:
    
    PluginGraph() {};
    ~PluginGraph();

    bool reset(double sampleRate);
    void process(std::vector<float>& inputFrame, size_t numInputChannels);
    bool canProcessAudioFrame();

    void initialiseDspObject(IAudioProcessor* dspObject);
    void addProcessingBlock(AudioProcessingBlockPtr processingBlock);
    void deleteProcessingBlock(AudioProcessingBlockPtr processingBlock);

    AudioProcessingBlockPtr generateProcessingBlock(DspObjectType type);

    bool isCreatingFeedback(AudioProcessingBlock* start, AudioProcessingBlock* end);

    void addInputs(int numInputs);
    void addOutputs(int numOutputs);

    Array<AudioProcessingBlockPtr> getInputs();
    Array<AudioProcessingBlockPtr> getOutputs();
    Array<AudioProcessingBlockPtr> getAllBlocks();

    int getNumInputs();
    int getNumOutputs();

    double getOutputSampleFrom(int outputChannel);
    double getLeftOutputSample();
    double getRightOutputSample();

    void setPluginState(XmlElementPtr pluginState);
    XmlElementPtr getPluginState();

    std::map<std::string, AudioProcessingBlockPtr> getAudioProcessingBlockMap();
    

    void clear();
    void deleteAndReplaceAudioBlocks(std::function<std::shared_ptr<AudioProcessorState>()> callback);
    void deleteAndReplaceAudioBlocks(std::shared_ptr<AudioProcessorState> state);

    //Do a pass of processing to generate process path
    void calculateProcessPath();
    void updateProcessPath();

private:
    void updateProcessors();
    void resetProcessors();
    Array<AudioProcessingBlock*> processBlocks(Array<AudioProcessingBlock*> blockToProcess);

    double sampleRate = 0;
    Array<AudioProcessingBlockPtr> allBlocks;
    Array<AudioProcessingBlockPtr> inputs;
    Array<AudioProcessingBlockPtr> outputs;

    XmlElementPtr pluginState = nullptr;
    
    bool updateProcessorsFlag = false;
    std::function<std::shared_ptr<AudioProcessorState>()> generateCallback;
    bool processPathNeedsUpdating = false;
    Array<Array<AudioProcessingBlock*>> processPath;

    std::shared_ptr<AudioProcessorState> tempProcessorState;
};
