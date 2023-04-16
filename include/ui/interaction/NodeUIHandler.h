/*
  ==============================================================================

    NodeUIHandler.h
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
class NodeUI;
class AudioProcessorNode;

#define NodeUIPtr std::shared_ptr<NodeUI>
#define AudioProcessorNodePtr std::shared_ptr<AudioProcessorNode>

class NodeUIHandler
{
public:
    NodeUIHandler() {};
    void initialize(GraphEditor* graphEditor, PluginGraph* pluginGraph);

    /// Creates an audio processor node with UI and AuidoProcessorNode
    NodeUIPtr createNode(AudioProcessorNodeType type, Point<int> position);
    /// Creates an audio processor node with UI and uses existing AudioProcessorNode
    NodeUIPtr createNode(AudioProcessorNodeType type, Point<int> position, AudioProcessorNodePtr processorNode);
    /// Adds NodeUI to processors list and displays in on the screen
    void initializeProcessor(NodeUIPtr processor);
    /// Removed UI from screen and removes AudioProcessorNode from plugin graph
    void deleteProcessor(NodeUI* processor);
    void duplicateProcessor(NodeUI* processor);

private:
    GraphEditor* graphEditor;
    PluginGraph* pluginGraph;
};