#pragma once
#include <dsp/IAudioProcessor.h>
#include <dsp/AudioProcessorNode.h>
#include <utils/ManagedArray.h>
#include <dsp/AudioProcessorState.h>
#include <utils/XmlUtils.h>

class PluginGraph : public ManagedArray
{
public:
    
    PluginGraph() {};
    ~PluginGraph();

    bool reset(double sampleRate);
    void process(std::vector<float>& inputFrame, size_t numInputChannels);
    bool canProcessAudioFrame();

    void createDSPObject(NodeInstance dspObjectType);
    void initialiseDspObject(IAudioProcessor* dspObject);
    void addProcessorNode(AudioProcessorNodePtr processorNode);
    void deleteProcessorNode(AudioProcessorNodePtr processorNode);

    AudioProcessorNodePtr generateProcessorNode(NodeInstance type);

    bool isCreatingFeedback(AudioProcessorNode* start, AudioProcessorNode* end);

    void addInputs(int numInputs);
    void addOutputs(int numOutputs);

    Array<AudioProcessorNodePtr> getInputs();
    Array<AudioProcessorNodePtr> getOutputs();
    Array<AudioProcessorNodePtr> getAllBlocks();

    int getNumInputs();
    int getNumOutputs();

    double getOutputSampleFrom(int outputChannel);
    double getLeftOutputSample();
    double getRightOutputSample();

    void setPluginState(XmlElementPtr pluginState);
    XmlElementPtr getPluginState();

    std::map<std::string, AudioProcessorNodePtr> getAudioProcessorNodeMap();

    void clear();
    void deleteAndReplaceAudioBlocks(std::function<std::shared_ptr<AudioProcessorState>()> callback);
    void deleteAndReplaceAudioBlocks(std::shared_ptr<AudioProcessorState> state);

    //Do a pass of processing to generate process path
    void calculateProcessPath();
    void updateProcessPath();

private:
    void updateProcessors();
    void resetProcessors();
    Array<AudioProcessorNode*> processBlocks(Array<AudioProcessorNode*> blockToProcess);

    double sampleRate = 0;
    Array<AudioProcessorNodePtr> allProcessorNodes;
    Array<AudioProcessorNodePtr> inputs;
    Array<AudioProcessorNodePtr> outputs;

    XmlElementPtr pluginState = nullptr;
    
    bool updateProcessorsFlag = false;
    std::function<std::shared_ptr<AudioProcessorState>()> generateCallback;
    bool processPathNeedsUpdating = false;
    Array<Array<AudioProcessorNode*>> processPath;

    std::shared_ptr<AudioProcessorState> tempProcessorState;
};
