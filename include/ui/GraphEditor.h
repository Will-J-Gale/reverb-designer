/*
  ==============================================================================

    GraphEditor.h
    Created: 13 Sep 2020 2:49:28pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include <ui/nodes/NodeConnection.h>
#include <utils/ManagedArray.h>
#include <ui/nodes/audioProcessors/Input.h>
#include <ui/nodes/audioProcessors/Output.h>
#include <ui/nodes/audioProcessors/BiquadFilterNode.h>
#include <ui/nodes/audioProcessors/SimpleDelayNode.h>
#include <utils/Constants.h>
#include <ui/nodes/audioProcessors/AllPassFilterNode.h>
#include <ui/menus/GraphEditorContextMenu.h>
#include <ui/interaction/SelectionHandler.h>
#include <ui/menus/MainMenu.h>
#include <dsp/AudioProcessorState.h>
#include <ui/interaction/ZoomHandler.h>
#include <ui/interaction/DragHandler.h>
#include <ui/interaction/ConnectionHandler.h>
#include <ui/interaction/NodeUIHandler.h>
#include <ui/nodes/NodeUI.h>

//Forward declarations
class NodeConnectorUI;
class GainNode;
class PluginGraph;

class GraphEditor : public Component,
                    public ManagedArray,
                    public NodeConnectorUI::Listener,
                    public MainMenu::Listener,
                    public NodeUI::Listener
{
public:
    GraphEditor();
    ~GraphEditor();

    void mouseDown(const MouseEvent& e) override;
    void mouseDrag(const MouseEvent& e) override;
    void mouseUp(const MouseEvent& e) override;
    void mouseWheelMove(const MouseEvent& event, const MouseWheelDetails& wheel) override;
    void paint(Graphics& g) override;

    void setPluginGraph(PluginGraph* pluginGraph);

    void resized() override;

    void duplicateSelectedProcessors();
    void reverseSelectedProcessors();
    void deleteSelectedProcessors();

    //Listener callbacks
    virtual void onProcessorClicked(NodeUI* processor, const MouseEvent& e) override;
    virtual void onProcessorMoved(NodeUI* processor, const MouseEvent& e) override;
    virtual void onProcessorReleased(NodeUI* processor, const MouseEvent& e) override;
    virtual void onContextSelection(NodeUI* processor, NodeUIConextMenuItems selection) override;

    virtual void onNodeLeftClick(NodeConnectorUI* nodeConnector, const MouseEvent& e) override;
    virtual void onNodeRightClick(NodeConnectorUI* nodeConnector, const MouseEvent& e) override;
    virtual void onNodeDrag(NodeConnectorUI* nodeConnector, const MouseEvent& e) override;
    virtual void onNodeLeftRelease(NodeConnectorUI* nodeConnector, const MouseEvent& e) override;
    
    // XmlElementPtr generatePluginState();
    void loadFromExistingState(XmlElement* state);
    std::shared_ptr<AudioProcessorState> loadStateFromFile(std::string xmlString);

    virtual void onSave(std::string savePath) override;
    virtual void onLoad(std::string filepath) override;
    virtual void onNewProject() override;
    virtual void onPresetSelected(Presets presetId) override;

    void clear();

private:
    void handleRightClick(const MouseEvent& e);
    void handleLeftClick(const MouseEvent& e);

    void drawConnections(Graphics& g);
    void drawPotentialConnection(Graphics& g);
    void addNodeConnectorListeners(Array<NodeConnectorUI*> nodes);
    void createIOProcessors();

    void createAllConnections(std::map<std::string, NodeUIPtr> processorUIMap, std::map<std::string, XmlElement*> xmlMap);

    NodeConnectorUI* getNodeConnectorAtPosition(Point<int> screenPos);
    Array<NodeUI*> getOverlappingProcessors(Rectangle<int> bounds);

    Array<NodeUIPtr> inputs;
    Array<NodeUIPtr> outputs;
    Array<NodeUIPtr> nodes;
    Array<NodeConnectionPtr> connections;
    NodeConnection potentialConnection;
    Array<NodeConnectorUI*> nodeConnectors;
    NodeConnectorUI* clickedNode = nullptr;

    PluginGraph* pluginGraph = nullptr;

    Point<int> mousePosition;
    Point<int> mouseDownPosition;
    Point<int> lastDragPosition;
    Rectangle<int> selectionArea;
    InteractionState interactionState = InteractionState::None;
    
    //Menus
    MainMenu mainMenu;
    GraphEditorContextMenu contextMenu;

    //Interaction
    friend class ConnectionHandler;
    friend class NodeUIHandler;
    SelectionHandler globalSelection;
    SelectionHandler selectionHandler;
    ZoomHandler zoomHandler = ZoomHandler(this);
    DragHandler dragHandler = DragHandler(this);
    ConnectionHandler connectionHandler; 
    NodeUIHandler processorNodeUIHandler; 
};