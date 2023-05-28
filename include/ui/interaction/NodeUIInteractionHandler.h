#pragma once
#include <JuceHeader.h>
#include <utils/Constants.h>
#include <ui/nodes/NodeUI.h>

class GraphEditor;
class PluginGraph;
class AudioProcessorNode;
class AudioProcessorMacroNode;

#define NodeUIPtr std::shared_ptr<NodeUI>
#define AudioProcessorNodePtr std::shared_ptr<AudioProcessorNode>

class NodeUIInteractionHandler : public NodeUI::Listener
{
public:
    NodeUIInteractionHandler() {};
    void initialize(GraphEditor* graphEditor, PluginGraph* pluginGraph);

    void onNodeClicked(NodeUI* processor, const MouseEvent& e) override;
    void onNodeMoved(NodeUI* processor, const MouseEvent& e) override;
    void onNodeReleased(NodeUI* processor, const MouseEvent& e) override;
    void onNodeContextSelection(NodeUI* processor, NodeContextMenuItems selection) override;
    
    NodeUIPtr createMacroNode(Point<int> position, String name);
    NodeUIPtr createMacroNodeFromXml(XmlElement* xml);
    NodeUIPtr createIONode(NodeInstance instance, Point<int> position, AudioProcessorNodePtr ioProcessorNode, int channel=0);
    NodeUIPtr createIONode(NodeInstance instance, Point<int> position, int channel=0);
    NodeUIPtr createNode(NodeInstance instance, Point<int> position);
    NodeUIPtr createNode(NodeInstance instance, Point<int> position, AudioProcessorNodePtr processorNode);

    void initializeNode(NodeUIPtr processor);
    void deleteNode(NodeUI* processor, bool deleteAudioNodes=false);
    void duplicateNode(NodeUI* processor);

private:
    GraphEditor* _graphEditor;
    PluginGraph* _pluginGraph;
};