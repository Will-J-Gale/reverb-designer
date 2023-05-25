#pragma once
#include <JuceHeader.h>

class AudioProcessorNode;
#define AudioProcessorNodePtr std::shared_ptr<AudioProcessorNode>

struct AudioProcessorState
{
    AudioProcessorState(
        Array<AudioProcessorNodePtr> newInputs,
        Array<AudioProcessorNodePtr> newOutputs,
        Array<AudioProcessorNodePtr> newNodes)
    {
        this->inputs = newInputs;
        this->outputs = newOutputs;
        this->nodes = newNodes;
    }

    Array<AudioProcessorNodePtr> inputs;
    Array<AudioProcessorNodePtr> outputs;
    Array<AudioProcessorNodePtr> nodes;
};