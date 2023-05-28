#pragma once
#include <dsp/AudioProcessorState.h>
#include <dsp/AudioProcessorNode.h>
#include <utils/ManagedArray.h>
#include <utils/Constants.h>
#include <utils/XmlUtils.h>
#include <ui/nodes/NodeConnection.h>
#include <ui/menus/GraphEditorContextMenu.h>
#include <ui/interaction/SelectionHandler.h>
#include <ui/menus/MainMenu.h>
#include <ui/interaction/ZoomHandler.h>
#include <ui/interaction/ConnectionHandler.h>
#include <ui/interaction/NodeUIInteractionHandler.h>
#include <ui/interaction/NodeConnectorInteractionHandler.h>
#include <ui/nodes/NodeUI.h>
#include <ui/nodes/audioProcessors/AudioProcessorMacroNode.h>

//Forward declarations
class PluginGraph;

class GraphEditor : public Component,
                    public ManagedArray
{
public:
    GraphEditor();
    GraphEditor(PluginGraph* pluginGraph, AudioProcessorMacroNode* parent=nullptr);
    ~GraphEditor();

    virtual void setPluginGraph(PluginGraph* pluginGraph);
    void addInputNode();
    void addOutputNode();
    
    Array<NodeConnectorUI*>& getNodeConnectors();
    Array<NodeUIPtr>& getNodes();
    Array<NodeUIPtr>& getInputs();
    Array<NodeUIPtr>& getOutputs();

    void clear();

    XmlElement* toXml();
    virtual void fromXml(XmlElement* xml, IdToAudioProcessorMap* idToProcessorMap=nullptr);

    void createAllConnections(std::map<std::string, NodeUIPtr> idToNodeUIMap, std::map<std::string, XmlElement*> idToXmlElement);

protected:
    void mouseDown(const MouseEvent& e) override;
    void mouseDrag(const MouseEvent& e) override;
    void mouseUp(const MouseEvent& e) override;
    void mouseWheelMove(const MouseEvent& event, const MouseWheelDetails& wheel) override;
    void handleRightClick(const MouseEvent& e);
    void handleLeftClick(const MouseEvent& e);
    void paint(Graphics& g) override;
    void duplicateSelectedProcessors();
    void reverseSelectedProcessors();
    void deleteSelectedProcessors();
    void drawConnections(Graphics& g);
    void drawPotentialConnection(Graphics& g);
    void addNodeConnectorListeners(Array<NodeConnectorUI*> nodes);

    Array<NodeUIPtr> _inputs;
    Array<NodeUIPtr> _outputs;
    Array<NodeUIPtr> _nodes;
    Array<NodeConnectionPtr> _connections;
    NodeConnection _potentialConnection;
    Array<NodeConnectorUI*> _nodeConnectors;
    NodeConnectorUI* _clickedNodeConnector = nullptr;
    PluginGraph* _pluginGraph = nullptr;
    AudioProcessorMacroNode* _parent = nullptr;
    Point<int> _mousePosition;
    Point<int> _mouseDownPosition;
    Point<int> _lastDragPosition;
    Rectangle<int> _selectionArea;
    InteractionState _interactionState = InteractionState::None;
    GraphEditorContextMenu _contextMenu;

    //Interaction
    friend class ConnectionHandler;
    friend class NodeUIInteractionHandler;
    friend class NodeConnectorInteractionHandler;
    
    //Used for moving all elements around with middle mouse button (emulating panning)
    SelectionHandler _globalSelection;
    //Used to only move selected items
    SelectionHandler _selectionHandler;
    ZoomHandler _zoomHandler{this};
    ConnectionHandler _connectionHandler; 
    NodeUIInteractionHandler _nodeInteractionHandler; 
    NodeConnectorInteractionHandler _nodeConnectorInteractionHandler{this};
};