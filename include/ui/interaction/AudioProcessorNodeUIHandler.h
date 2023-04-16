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
class PluginGraph;
class AudioProcessorNodeUI;
class AudioProcessorNode;

#define AudioProcessorNodeUIPtr std::shared_ptr<AudioProcessorNodeUI>
#define AudioProcessorNodePtr std::shared_ptr<AudioProcessorNode>

class AudioProcessorNodeUIHandler
{
public:
    AudioProcessorNodeUIHandler() {};
    void initialize(GraphEditor* graphEditor, PluginGraph* pluginGraph);

    /// Creates an audio processor node with UI and AuidoProcessorNode
    AudioProcessorNodeUIPtr createAudioProcessorNode(AudioProcessorNodeType type, Point<int> position);
    /// Creates an audio processor node with UI and uses existing AudioProcessorNode
    AudioProcessorNodeUIPtr createAudioProcessorNode(AudioProcessorNodeType type, Point<int> position, AudioProcessorNodePtr processorNode);
    /// Creates just the UI for an AudioProcessorUI
    AudioProcessorNodeUIPtr createAudioProcessorNodeUI(AudioProcessorNodeType type, Point<int> position);
    /// Adds AudioProcessorNodeUI to processors list and displays in on the screen
    void initializeProcessor(AudioProcessorNodeUIPtr processor);
    /// Removed UI from screen and removes AudioProcessorNode from plugin graph
    void deleteProcessor(AudioProcessorNodeUI* processor);
    void duplicateProcessor(AudioProcessorNodeUI* processor);

private:
    GraphEditor* graphEditor;
    PluginGraph* pluginGraph;
};