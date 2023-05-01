#pragma once
#include <JuceHeader.h>

class AudioProcessorNode;
#define AudioProcessorNodePtr std::shared_ptr<AudioProcessorNode>

struct AudioProcessorState
{
    AudioProcessorState(
        Array<AudioProcessorNodePtr> inputs,
        Array<AudioProcessorNodePtr> outputs,
        Array<AudioProcessorNodePtr> allBlocks)
    {
        this->inputs = inputs;
        this->outputs = outputs;
        this->allBlocks = allBlocks;
    }

    Array<AudioProcessorNodePtr> inputs;
    Array<AudioProcessorNodePtr> outputs;
    Array<AudioProcessorNodePtr> allBlocks;
};