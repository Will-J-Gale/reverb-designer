/*
  ==============================================================================

    AudioProcessingBlock.h
    Created: 20 Sep 2020 4:16:08pm
    Author:  Will

  ==============================================================================
*/

#pragma once

#include "IAudioProcessor.h"
#include <JuceHeader.h>
#include <memory>
#include "Constants.h"

#define AudioProcessingBlockPtr std::shared_ptr<AudioProcessingBlock>

class AudioProcessingBlock
{
public:
    AudioProcessingBlock() {};
    AudioProcessingBlock(DspObjectType type) { this->type = type; };
    ~AudioProcessingBlock();

    void process(double xn);
    void process();

    void reset();

    void connectInput(AudioProcessingBlock* connection);
    void connectFeedbackInput(AudioProcessingBlock* connection);
    void connectOutput(AudioProcessingBlock* connection);
    void disconnectInput(AudioProcessingBlock* connection);
    void disconnectOutput(AudioProcessingBlock* connection);

    bool isReady();

    void setProcessor(IAudioProcessorPtr processor);
    IAudioProcessor* getProcessor();

    double getInputSample();
    double getOutputSample();
    void setOutputSample(double sample);

    Array<AudioProcessingBlock*> getInputConnections();
    Array<AudioProcessingBlock*> getOutputConnections();
    Array<AudioProcessingBlock*> getReadyOutputConnections();

    bool hasFinishedProcessing();
    void setFinishedProcessing(bool hasFinished);

    std::string getIdAsString();

    DspObjectType getType() { return type; }

private:
    Uuid id;
    IAudioProcessorPtr processor = nullptr;

    Array<AudioProcessingBlock*> inputs;
    Array<AudioProcessingBlock*> outputs;
    Array<AudioProcessingBlock*> feedbackConnections;

    DspObjectType type;

    double xn = 0.0;
    double yn = 0.0;

    bool finishedProcessing = false;
    bool isFeedback = false;
};