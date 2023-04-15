/*
  ==============================================================================

    ProcessorUIHandler.h
    Created: 26 Oct 2020 9:45:05pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include <memory>
#include <JuceHeader.h>
#include "Constants.h"

class GraphEditor;
class AudioProcessorUI;
class AudioProcessingBlock;

#define AudioProcessorUIPtr std::shared_ptr<AudioProcessorUI>
#define AudioProcessingBlockPtr std::shared_ptr<AudioProcessingBlock>

class ProcessorUIHandler
{
public:
    ProcessorUIHandler(GraphEditor* graphEditor);

    AudioProcessorUIPtr createDspObject(DspObjectType type, Point<int> position);
    AudioProcessorUIPtr createDspObject(DspObjectType type, Point<int> position, AudioProcessingBlockPtr processingBlock);
    void initializeProcessor(AudioProcessorUIPtr processor);
    void deleteProcessor(AudioProcessorUI* processor);
    void duplicateProcessor(AudioProcessorUI* processor);

private:
    GraphEditor* graphEditor;
};