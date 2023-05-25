#pragma once

#include <dsp/IAudioProcessor.h>
#include <JuceHeader.h>
#include <memory>
#include <utils/Constants.h>
#include <dsp/PassThrough.h>
#include <parameters/AudioParameters.h>

#define AudioProcessorNodePtr std::shared_ptr<AudioProcessorNode>
#define IdToAudioProcessorMap std::map<std::string, AudioProcessorNodePtr>

/// AudioProcessorNode wraps an IAudioProcessor and handles input, output
/// and feedback to other AudioProcessorNodes
class AudioProcessorNode
{
public:
    AudioProcessorNode() {};
    AudioProcessorNode(NodeInstance nodeInstance) { _nodeInstance = nodeInstance; };
    ~AudioProcessorNode();

    void process(double xn);
    void process();
    void reset();
    void connectInput(AudioProcessorNode* connection);
    void connectFeedbackInput(AudioProcessorNode* connection);
    void connectOutput(AudioProcessorNode* connection);
    void disconnectInput(AudioProcessorNode* connection);
    void disconnectOutput(AudioProcessorNode* connection);
    void disconnectAll();
    bool isReady();
    void setProcessor(IAudioProcessorPtr processor);
    IAudioProcessor* getProcessor();
    double getInputSample();
    double getOutputSample();
    void setOutputSample(double sample);
    Array<AudioProcessorNode*> getInputConnections();
    Array<AudioProcessorNode*> getOutputConnections();
    Array<AudioProcessorNode*> getReadyOutputConnections();
    bool hasFinishedProcessing();
    void setFinishedProcessing(bool hasFinished);
    std::string getIdAsString();
    NodeInstance getNodeInstance() { return _nodeInstance; }
    AudioParametersPtr getParameters() { return _processor->getParameters(); };
    // void setParameters(AudioParametersPtr parameters) { processor->setParameters(parameters); };

private:
    Uuid _id;
    IAudioProcessorPtr _processor = std::make_shared<PassThrough>();
    Array<AudioProcessorNode*> _inputs;
    Array<AudioProcessorNode*> _outputs;
    Array<AudioProcessorNode*> _feedbackConnections;
    NodeInstance _nodeInstance;
    double _xn = 0.0;
    double _yn = 0.0;
    bool _finishedProcessing = false;
    bool _isFeedback = false;
};