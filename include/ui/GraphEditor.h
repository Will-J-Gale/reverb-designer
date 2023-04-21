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
#include <ui/interaction/NodeUIInteractionHandler.h>
#include <ui/interaction/NodeConnectorInteractionHandler.h>
#include <ui/interaction/MainMenuInteractionHandler.h>
#include <ui/nodes/NodeUI.h>

//Forward declarations
class NodeConnectorUI;
class GainNode;
class PluginGraph;

class GraphEditor : public Component,
                    public ManagedArray
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

    // XmlElementPtr generatePluginState();
    void loadFromExistingState(XmlElement* state);
    std::shared_ptr<AudioProcessorState> loadStateFromFile(std::string xmlString);

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
    NodeConnectorUI* clickedNodeConnector = nullptr;

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
    friend class NodeUIInteractionHandler;
    friend class NodeConnectorInteractionHandler;
    friend class MainMenuInteractionHandler;
    
    SelectionHandler globalSelection;
    SelectionHandler selectionHandler;
    ZoomHandler zoomHandler{this};
    DragHandler dragHandler{this};
    ConnectionHandler connectionHandler; 
    NodeUIInteractionHandler nodeInteractionHandler; 
    NodeConnectorInteractionHandler nodeConnectorInteractionHandler{this};
    MainMenuInteractionHandler mainMenuInteractionHandler{this};
};