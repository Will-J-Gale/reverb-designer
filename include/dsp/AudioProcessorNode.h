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
    AudioProcessorNode(DspObjectType type) { this->type = type; };
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
    DspObjectType getType() { return type; }

private:
    Uuid id;
    IAudioProcessorPtr processor = nullptr;
    Array<AudioProcessorNode*> inputs;
    Array<AudioProcessorNode*> outputs;
    Array<AudioProcessorNode*> feedbackConnections;
    DspObjectType type;
    double xn = 0.0;
    double yn = 0.0;
    bool finishedProcessing = false;
    bool isFeedback = false;
};