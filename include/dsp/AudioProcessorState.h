#pragma once
#include <JuceHeader.h>

class AudioProcessorNode;
#define AudioProcessorNodePtr std::shared_ptr<AudioProcessorNode>

struct AudioProcessorState
{
    AudioProcessorState(
        Array<AudioProcessorNodePtr> inputs,
        Array<AudioProcessorNodePtr> outputs,
        Array<AudioProcessorNodePtr> nodes)
    {
        this->inputs = inputs;
        this->outputs = outputs;
        this->nodes = nodes;
    }

    Array<AudioProcessorNodePtr> inputs;
    Array<AudioProcessorNodePtr> outputs;
    Array<AudioProcessorNodePtr> nodes;
};