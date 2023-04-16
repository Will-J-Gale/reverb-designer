/*
  ==============================================================================

    GraphEditor.h
    Created: 13 Sep 2020 2:49:28pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include <ui/audioProcessorNodes/AudioProcessorNodeUI.h>
#include <ui/audioProcessorNodes/NodeConnection.h>
#include <utils/ManagedArray.h>
#include <ui/audioProcessorNodes/Input.h>
#include <ui/audioProcessorNodes/Output.h>
#include <ui/audioProcessorNodes/BiquadFilterNode.h>
#include <ui/audioProcessorNodes/SimpleDelayNode.h>
#include <utils/Constants.h>
#include <ui/audioProcessorNodes/AllPassFilterNode.h>
#include <ui/menus/GraphEditorContextMenu.h>
#include <ui/interaction/SelectionHandler.h>
#include <ui/menus/MainMenu.h>
#include <dsp/AudioProcessorState.h>
#include <ui/interaction/ZoomHandler.h>
#include <ui/interaction/DragHandler.h>
#include <ui/interaction/ConnectionHandler.h>
#include <ui/interaction/AudioProcessorNodeUIHandler.h>

//Forward declarations
class Node;
class GainNode;
class PluginGraph;

class GraphEditor : public Component,
                    public ManagedArray,
                    public AudioProcessorNodeUI::Listener,
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
    virtual void onProcessorClicked(AudioProcessorNodeUI* processor, const MouseEvent& e) override;
    virtual void onProcessorMoved(AudioProcessorNodeUI* processor, const MouseEvent& e) override;
    virtual void onProcessorReleased(AudioProcessorNodeUI* processor, const MouseEvent& e) override;
    virtual void onContextSelection(AudioProcessorNodeUI* processor, AudioProcessorConextMenuItems selection) override;

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

    void createAllConnections(std::map<std::string, AudioProcessorNodeUIPtr> processorUIMap, std::map<std::string, XmlElement*> xmlMap);

    Node* getNodeAtPosition(Point<int> screenPos);
    Array<AudioProcessorNodeUI*> getOverlappingProcessors(Rectangle<int> bounds);

    Array<AudioProcessorNodeUIPtr> inputs;
    Array<AudioProcessorNodeUIPtr> outputs;
    Array<AudioProcessorNodeUIPtr> processors;
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
    friend class AudioProcessorNodeUIHandler;
    SelectionHandler globalSelection;
    SelectionHandler selectionHandler;
    ZoomHandler zoomHandler = ZoomHandler(this);
    DragHandler dragHandler = DragHandler(this);
    ConnectionHandler connectionHandler = ConnectionHandler(this);
    AudioProcessorNodeUIHandler processorUIHandler; 
};