/*
  ==============================================================================

    AudioProcessorNodeUIHandler.h
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

class AudioProcessorNodeUIHandler
{
public:
    AudioProcessorNodeUIHandler(GraphEditor* graphEditor);

    AudioProcessorNodeUIPtr createAudioProcessorNode(AudioProcessorNodeType type, Point<int> position);
    AudioProcessorNodeUIPtr createAudioProcessorNode(AudioProcessorNodeType type, Point<int> position, AudioProcessorNodePtr processorNode);
    void initializeProcessor(AudioProcessorNodeUIPtr processor);
    void deleteProcessor(AudioProcessorNodeUI* processor);
    void duplicateProcessor(AudioProcessorNodeUI* processor);

private:
    GraphEditor* graphEditor;
};