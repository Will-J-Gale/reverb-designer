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
class AudioProcessorNodeUI;
class AudioProcessorNode;

#define AudioProcessorNodeUIPtr std::shared_ptr<AudioProcessorNodeUI>
#define AudioProcessorNodePtr std::shared_ptr<AudioProcessorNode>

class ProcessorUIHandler
{
public:
    ProcessorUIHandler(GraphEditor* graphEditor);

    AudioProcessorNodeUIPtr createDspObject(DspObjectType type, Point<int> position);
    AudioProcessorNodeUIPtr createDspObject(DspObjectType type, Point<int> position, AudioProcessorNodePtr processingBlock);
    void initializeProcessor(AudioProcessorNodeUIPtr processor);
    void deleteProcessor(AudioProcessorNodeUI* processor);
    void duplicateProcessor(AudioProcessorNodeUI* processor);

private:
    GraphEditor* graphEditor;
};