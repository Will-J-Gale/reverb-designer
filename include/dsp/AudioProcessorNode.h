/*
  ==============================================================================

    AudioProcessorNode.h
    Created: 20 Sep 2020 4:16:08pm
    Author:  Will

  ==============================================================================
*/

#pragma once

#include <dsp/IAudioProcessor.h>
#include <JuceHeader.h>
#include <memory>
#include <utils/Constants.h>

#define AudioProcessorNodePtr std::shared_ptr<AudioProcessorNode>

/// AudioProcessorNode wraps an IAudioProcessor and handles input, output
/// and feedback to other AudioProcessorNodes
class AudioProcessorNode
{
public:
    AudioProcessorNode() {};
    AudioProcessorNode(NodeInstance nodeInstance) { this->nodeInstance = nodeInstance; };
    ~AudioProcessorNode();

    void process(double xn);
    void process();
    void reset();
    void connectInput(AudioProcessorNode* connection);
    void connectFeedbackInput(AudioProcessorNode* connection);
    void connectOutput(AudioProcessorNode* connection);
    void disconnectInput(AudioProcessorNode* connection);
    void disconnectOutput(AudioProcessorNode* connection);
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
    NodeInstance getNodeInstance() { return nodeInstance; }

private:
    Uuid id;
    IAudioProcessorPtr processor = nullptr;
    Array<AudioProcessorNode*> inputs;
    Array<AudioProcessorNode*> outputs;
    Array<AudioProcessorNode*> feedbackConnections;
    NodeInstance nodeInstance;
    double xn = 0.0;
    double yn = 0.0;
    bool finishedProcessing = false;
    bool isFeedback = false;
};