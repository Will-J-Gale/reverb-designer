/*
  ==============================================================================

    AudioProcessorState.h
    Created: 24 Oct 2020 1:54:40pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class AudioProcessingBlock;
#define AudioProcessingBlockPtr std::shared_ptr<AudioProcessingBlock>

struct AudioProcessorState
{
    AudioProcessorState(
        Array<AudioProcessingBlockPtr> inputs,
        Array<AudioProcessingBlockPtr> outputs,
        Array<AudioProcessingBlockPtr> allBlocks)
    {
        this->inputs = inputs;
        this->outputs = outputs;
        this->allBlocks = allBlocks;
    }

    Array<AudioProcessingBlockPtr> inputs;
    Array<AudioProcessingBlockPtr> outputs;
    Array<AudioProcessingBlockPtr> allBlocks;
};