#pragma once
#include <dsp/IAudioProcessor.h>
#include <dsp/AudioProcessorNode.h>
#include <dsp/AudioProcessorState.h>
#include <utils/ManagedArray.h>
#include <utils/XmlUtils.h>
#include <utils/Constants.h>

class PluginGraph : public ManagedArray
{
public:
    
    PluginGraph() {};
    ~PluginGraph();

    void reset(double sampleRate);
    void process(std::vector<float>& inputFrame, int numInputChannels);
    bool canProcessAudioFrame();

    void createDSPObject(NodeInstance dspObjectType);
    void initialiseDspObject(IAudioProcessor* dspObject);
    void addProcessorNode(AudioProcessorNodePtr processorNode);
    void deleteProcessorNode(AudioProcessorNode* processorNode);
    void deleteProcessorNodes(Array<AudioProcessorNode*> processorNodes);

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
    // void deleteAndReplaceAudioBlocks(std::function<std::shared_ptr<AudioProcessorState>()> callback);
    // void deleteAndReplaceAudioBlocks(std::shared_ptr<AudioProcessorState> state);

    //Do a pass of processing to generate process path
    void calculateProcessPath();
    void addAction(PluginGraphActionType actionType, PluginGraphActionCallback actionCallback=nullptr);

private:
    void updateProcessors();
    void resetProcessors();
    void performDelete();
    bool handleActions();
    Array<AudioProcessorNode*> processBlocks(Array<AudioProcessorNode*> blockToProcess);

private:

    double _sampleRate = DEFAULT_SAMPLERATE;
    Array<AudioProcessorNodePtr> _allNodes;
    Array<AudioProcessorNodePtr> _inputs;
    Array<AudioProcessorNodePtr> _outputs;
    Array<AudioProcessorNode*> _nodesToDelete;

    XmlElementPtr _pluginState = nullptr;
    
    std::function<std::shared_ptr<AudioProcessorState>()> _generateCallback;
    Array<Array<AudioProcessorNode*>> _processPath;
    std::shared_ptr<AudioProcessorState> _tempProcessorState;

    std::queue<PluginGraphAction> actions;
    CriticalSection objectLock;
};
