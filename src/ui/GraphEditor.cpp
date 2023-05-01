/*
  ==============================================================================

    GraphEditor.cpp
    Created: 13 Sep 2020 2:49:28pm
    Author:  Will

  ==============================================================================
*/

#include <map>
#include <ui/GraphEditor.h>
#include <dsp/PluginGraph.h>
#include <ui/interaction/NodeConnectionDrawer.h>
#include <utils/HitTest.h>
#include <utils/XmlGenerator.h>
#include <utils/StorageManager.h>
#include <utils/PresetFactory.h>
#include <utils/HitTest.h>

GraphEditor::GraphEditor()
{
    setBounds(0, 0, PLUGIN_WIDTH, PLUGIN_HEIGHT);
    zoomHandler.setOriginalBounds(getBounds());
}

GraphEditor::GraphEditor(PluginGraph* pluginGraph)
{
    setBounds(0, 0, PLUGIN_WIDTH, PLUGIN_HEIGHT);
    zoomHandler.setOriginalBounds(getBounds());
    setPluginGraph(pluginGraph);
}

GraphEditor::~GraphEditor()
{
}

void GraphEditor::mouseDown(const MouseEvent& e)
{
    Array<NodeUI*> tempNodes;
    for (auto node : nodes)
    {
        tempNodes.add(node.get());
    }

    globalSelection.setItems(tempNodes);

    if (e.mods.isRightButtonDown())
        handleRightClick(e);
    else if(e.mods.isLeftButtonDown())
        handleLeftClick(e);
}

void GraphEditor::mouseDrag(const MouseEvent& e)
{
    if (interactionState == InteractionState::CreateSelection)
    {
        mousePosition = e.getPosition();
        auto width = abs(mouseDownPosition.x - mousePosition.x);
        auto height = abs(mouseDownPosition.y - mousePosition.y);
        auto topLeft = Point<int>(fmin(mousePosition.x, mouseDownPosition.x), fmin(mousePosition.y, mouseDownPosition.y));
        
        selectionArea.setBounds(topLeft.x, topLeft.y, width, height);
        selectionHandler.setItems(HitTest::getOverlappingNodes(selectionArea, nodes));
    }
    else if (e.mods.isMiddleButtonDown())
        globalSelection.moveItems(e.getOffsetFromDragStart());

    repaint();
}

void GraphEditor::mouseUp(const MouseEvent& e)
{
    globalSelection.updateItemPositions();
    interactionState = InteractionState::None;
    selectionArea.setSize(0, 0);
    repaint();
}

void GraphEditor::mouseWheelMove(const MouseEvent& event, const MouseWheelDetails& wheel)
{
    if (wheel.deltaY > 0)
        zoomHandler.zoomIn();
    else
        zoomHandler.zoomOut();
}

void GraphEditor::paint(Graphics& g)
{
    g.fillAll(Colour::fromString(BACKGROUND_COLOUR_STRING));
    drawConnections(g);

    if (interactionState == InteractionState::CreateSelection)
    {
        g.setColour(Colour::fromString(SELECTION_AREA_COLOUR));
        g.fillRect(selectionArea);
    }

    if (selectionHandler.size() > 0)
    {
        for (auto processor : selectionHandler.getItems())
        {
            auto procPosition = processor->getPosition();
            auto procWidth = processor->getWidth() * SELECTION_SCALE;
            auto procHeight = processor->getHeight() * SELECTION_SCALE;

            g.setColour(Colour::fromString(SELECTED_PROCESSOR_COLOUR));
            g.drawRoundedRectangle(Rectangle<int>(procPosition.x, procPosition.y, procWidth, procHeight).toFloat(), CORNER_ROUNDNESS, SELECTION_THICKNESS);
        }
    }
}

void GraphEditor::setPluginGraph(PluginGraph* pluginGraph)
{
    this->pluginGraph = pluginGraph;
    nodeInteractionHandler.initialize(this, pluginGraph);
    connectionHandler.initialize(this, pluginGraph);
}



void GraphEditor::drawConnections(Graphics& g)
{
    if (clickedNodeConnector != nullptr)
        drawPotentialConnection(g);

    g.setColour(Colour::fromString(CONNECTION_COLOUR));

    for (auto connection : connections)
        NodeConnectionDrawer::drawConnection(g, this, connection->getStartConnector(), connection->getEndConnector());
}

void GraphEditor::drawPotentialConnection(Graphics& g)
{
    if (interactionState == InteractionState::DragConnection)
    {
        g.setColour(Colour::fromString(CONNECTION_COLOUR));

        Point<int> pos = clickedNodeConnector->getScreenPosition();
        pos = getLocalPoint(nullptr, pos);

        auto processor = (NodeUI*)(clickedNodeConnector->getParentComponent());
        auto isReversed = processor->isReversed();

        int x1 = pos.getX() + NODE_RADIUS;
        int y1 = pos.getY() + NODE_RADIUS;
        int x2 = mousePosition.getX();
        int y2 = mousePosition.getY();

        if (clickedNodeConnector->getType() == NodeConnectorType::AudioInput)
        {
            x1 = mousePosition.getX();
            y1 = mousePosition.getY();
            x2 = pos.getX();
            y2 = pos.getY();
        }

        NodeConnectionDrawer::drawRightAngleConnection(g, Point<int>(x1, y1), Point<int>(x2, y2));
    }
}

void GraphEditor::addNodeConnectorListeners(Array<NodeConnectorUI*> nodeConnectors)
{
    for (auto nodeConnector : nodeConnectors)
        nodeConnector->addListener(&nodeConnectorInteractionHandler);
}

void GraphEditor::addInputNode()
{
    auto newInput = nodeInteractionHandler.createNode(NodeInstance::Input, Point<int>());
    nodeInteractionHandler.initializeNode(newInput);
    inputs.add(newInput);
    newInput->setTopLeftPosition(INPUT_START_X, IO_START_Y);
}

void GraphEditor::addOutputNode()
{
    auto newOutput = nodeInteractionHandler.createNode(NodeInstance::Output, Point<int>());
    nodeInteractionHandler.initializeNode(newOutput);
    newOutput->setTopLeftPosition(OUTPUT_START_X, IO_START_Y);
    outputs.add(newOutput);
}

void GraphEditor::handleRightClick(const MouseEvent& e)
{
    int contextSelection = contextMenu.show();

    if (contextSelection == (int)GraphEditorContextMenuItems::Duplicate)
    {
        duplicateSelectedProcessors();
    }
    else if (contextSelection == (int)GraphEditorContextMenuItems::Macro)
    {
        //Duplicate code - Same as below
        auto node = nodeInteractionHandler.createMacroNode(e.getPosition());
        nodeInteractionHandler.initializeNode(node);
    }
    else if (contextSelection > 0)
    {
        auto node = nodeInteractionHandler.createNode((NodeInstance)contextSelection, e.getPosition());
        nodeInteractionHandler.initializeNode(node);
    }
}

void GraphEditor::handleLeftClick(const MouseEvent& e)
{
    selectionHandler.clear();
    mouseDownPosition = e.getPosition();
    interactionState = InteractionState::CreateSelection; 
}

void GraphEditor::duplicateSelectedProcessors()
{
    for (auto node : selectionHandler.getItems())
    {
        NodeInstance nodeInstance = node->getNodeInstance();
        if (nodeInstance == NodeInstance::Input || nodeInstance == NodeInstance::Output)
            continue;

        nodeInteractionHandler.duplicateProcessor(node);
    }
}

void GraphEditor::reverseSelectedProcessors()
{
    for (auto processor : selectionHandler.getItems())
    {
        if (arrayContains(inputs, processor) || arrayContains(outputs, processor))
            continue;

        processor->reverse();
    }
}

void GraphEditor::deleteSelectedProcessors()
{
    for (auto node : selectionHandler.getItems())
    {
        NodeInstance nodeInstance = node->getNodeInstance();
        if (nodeInstance == NodeInstance::Input || nodeInstance == NodeInstance::Output)
            continue;

        nodeInteractionHandler.deleteProcessor(node);
    }

    selectionHandler.clear();
}

Array<NodeConnectorUI*>& GraphEditor::getNodeConnectors()
{
    return nodeConnectors;
}
Array<NodeUIPtr>& GraphEditor::getNodes()
{
    return nodes;
}

Array<NodeUIPtr>& GraphEditor::getInputs()
{
    return inputs;
}


Array<NodeUIPtr>& GraphEditor::getOutputs()
{
    return outputs;
}

void GraphEditor::clear()
{
    inputs.clear();
    outputs.clear();
    nodes.clear();
    nodeConnectors.clear();
    connections.clear();
    globalSelection.clear();
    selectionHandler.clear();
}