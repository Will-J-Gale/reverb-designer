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
#include <utils/Constants.h>

class GraphEditor;
class AudioProcessorNode;
class AudioProcessingBlock;

#define AudioProcessorNodePtr std::shared_ptr<AudioProcessorNode>
#define AudioProcessingBlockPtr std::shared_ptr<AudioProcessingBlock>

class ProcessorUIHandler
{
public:
    ProcessorUIHandler(GraphEditor* graphEditor);

    AudioProcessorNodePtr createDspObject(DspObjectType type, Point<int> position);
    AudioProcessorNodePtr createDspObject(DspObjectType type, Point<int> position, AudioProcessingBlockPtr processingBlock);
    void initializeProcessor(AudioProcessorNodePtr processor);
    void deleteProcessor(AudioProcessorNode* processor);
    void duplicateProcessor(AudioProcessorNode* processor);

private:
    GraphEditor* graphEditor;
};