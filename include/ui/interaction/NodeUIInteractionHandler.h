#pragma once
#include <memory>
#include <JuceHeader.h>
#include <utils/Constants.h>
#include <ui/nodes/NodeUI.h>

class GraphEditor;
class PluginGraph;
class AudioProcessorNode;

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
    void onNodeContextSelection(NodeUI* processor, NodeUIConextMenuItems selection) override;
    
    NodeUIPtr createMacroNode(Point<int> position, String name);
    NodeUIPtr createNode(NodeInstance type, Point<int> position);
    NodeUIPtr createNode(NodeInstance type, Point<int> position, AudioProcessorNodePtr processorNode);
    NodeUIPtr createNodeFromXml(NodeInstance type, XmlElement* xml);

    void initializeNode(NodeUIPtr processor);
    void deleteProcessor(NodeUI* processor);
    void duplicateProcessor(NodeUI* processor);

private:
    GraphEditor* graphEditor;
    PluginGraph* pluginGraph;
};