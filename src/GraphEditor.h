/*
  ==============================================================================

    GraphEditor.h
    Created: 13 Sep 2020 2:49:28pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include "AudioProcessorUI.h"
#include "NodeConnection.h"
#include "ManagedArray.h"
#include "Input.h"
#include "Output.h"
#include "FilterUI.h"
#include "SimpleDelayUI.h"
#include "Constants.h"
#include "AllPassFilterUI.h"
#include "GraphEditorContextMenu.h"
#include "SelectionHandler.h"
#include "MainMenu.h"
#include "AudioProcessorState.h"
#include "ZoomHandler.h"
#include "DragHandler.h"
#include "ConnectionHandler.h"
#include "ProcessorUIHandler.h"

//Forward declarations
class Node;
class GainUI;
class PluginGraph;
class XmlGenerator;

class GraphEditor : public Component,
                    public ManagedArray,
                    public AudioProcessorUI::Listener,
                    public Node::Listener,
                    public MainMenu::Listener
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
    virtual void onProcessorClicked(AudioProcessorUI* processor, const MouseEvent& e) override;
    virtual void onProcessorMoved(AudioProcessorUI* processor, const MouseEvent& e) override;
    virtual void onProcessorReleased(AudioProcessorUI* processor, const MouseEvent& e) override;
    virtual void onContextSelection(AudioProcessorUI* processor, AudioProcessorConextMenuItems selection) override;

    virtual void onNodeLeftClick(Node* node, const MouseEvent& e) override;
    virtual void onNodeRightClick(Node* node, const MouseEvent& e) override;
    virtual void onNodeDrag(Node* node, const MouseEvent& e) override;
    virtual void onNodeLeftRelease(Node* node, const MouseEvent& e) override;
    
    XmlElementPtr generatePluginState();
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
    void addNodeListeners(Array<Node*> nodes);
    void createIOProcessors();

    void createAllConnections(std::map<std::string, AudioProcessorUIPtr> processorUIMap, std::map<std::string, XmlElement*> xmlMap);

    Node* getNodeAtPosition(Point<int> screenPos);
    Array<AudioProcessorUI*> getOverlappingProcessors(Rectangle<int> bounds);

    Array<AudioProcessorUIPtr> inputs;
    Array<AudioProcessorUIPtr> outputs;
    Array<AudioProcessorUIPtr> processors;
    Array<NodeConnectionPtr> connections;
    NodeConnection potentialConnection;
    Array<Node*> nodes;
    Node* clickedNode = nullptr;

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
    friend class ProcessorUIHandler;
    SelectionHandler globalSelection;
    SelectionHandler selectionHandler;
    ZoomHandler zoomHandler = ZoomHandler(this);
    DragHandler dragHandler = DragHandler(this);
    ConnectionHandler connectionHandler = ConnectionHandler(this);
    ProcessorUIHandler processorUIHandler = ProcessorUIHandler(this);
};