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
#include <ui/nodes/audioProcessors/GainNode.h>
#include <ui/nodes/audioProcessors/AudioProcessorNodeUIFactory.h>
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
    // auto pluginState = pluginGraph->getPluginState();
    // nodeInteractionHandler.initialize(this, pluginGraph);
    // connectionHandler.initialize(this, pluginGraph);

    // if (pluginState != nullptr)
    //     loadFromExistingState(pluginState.get());
    // else
    //     createIOProcessors();
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

}

void GraphEditor::addOutputNode()
{

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

std::shared_ptr<AudioProcessorState> GraphEditor::loadStateFromFile(std::string xmlString)
{
    // const MessageManagerLock mmlock;

    // auto xml = parseXML(xmlString);
    // auto state = xml.get();

    // if (state == nullptr || state->getTagName() != PLUGIN_STATE_TAG)
    // {
    //     return nullptr;
    // }
    
    // auto tempInputs = Array<AudioProcessorNodePtr>();
    // auto tempOutputs = Array<AudioProcessorNodePtr>();
    // auto tempAllBlocks = Array<AudioProcessorNodePtr>();

    // auto xmlMap = XmlGenerator::generateXmlMap(state);

    // std::map<std::string, NodeUIPtr> processorUIMap;

    // //Generate all processing UI
    // for (auto pair : xmlMap)
    // {
    //     auto id = pair.first;
    //     auto xml = pair.second;
    //     auto type = (NodeType)(xml->getChildByName(TYPE_TAG)->getAllSubText().getIntValue());
    //     auto x = xml->getChildByName(X_TAG)->getAllSubText().getIntValue();
    //     auto y = xml->getChildByName(Y_TAG)->getAllSubText().getIntValue();
    //     auto isReversed = xml->getChildByName(IS_REVERSED_TAG)->getAllSubText().getIntValue();
    //     auto parametersXML = xml->getChildByName(PARAMETERTS_TAG);

    //     NodeUIPtr processorUI;

    //     if (type == NodeType::Input)
    //     {
    //         auto channel = parametersXML->getChildByName(IO_CHANNEL_TAG)->getAllSubText().getIntValue();

    //         processorUI = processorNodeUIInteractionHandler.createNode(NodeType::Input, Point<int>(x, y));
    //         processorUI->setAudioParametersFromXml(parametersXML);

    //         inputs.add(processorUI);
    //         tempInputs.add(processorUI->getProcessorNode());
    //     }
    //     else if (type == NodeType::Output)
    //     {
    //         auto channel = parametersXML->getChildByName(IO_CHANNEL_TAG)->getAllSubText().getIntValue();

    //         processorUI = processorNodeUIInteractionHandler.createNode(NodeType::Output, Point<int>(x, y));
    //         processorUI->setAudioParametersFromXml(parametersXML);

    //         outputs.add(processorUI);
    //         tempOutputs.add(processorUI->getProcessorNode());
    //     }
    //     else //Processor does not exist
    //     {
    //         processorUI = processorNodeUIInteractionHandler.createNode(type, Point<int>(x, y));
    //         processorUI->setAudioParametersFromXml(parametersXML);

    //         tempAllBlocks.add(processorUI->getProcessorNode());
    //     }

    //     if (isReversed)
    //         processorUI->reverse();

    //     processorNodeUIInteractionHandler.initializeProcessor(processorUI);

    //     processorUIMap[id] = processorUI;
    // }

    // createAllConnections(processorUIMap, xmlMap);

    // xml->deleteAllChildElements();

    // repaint();
    // return std::make_unique<AudioProcessorState>(tempInputs, tempOutputs, tempAllBlocks);
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

void GraphEditor::loadFromExistingState(XmlElement* state)
{
    // //Load xml 
    // if (state == nullptr || state->getTagName() != PLUGIN_STATE_TAG)
    // {
    //     return;
    // }
    
    // auto xmlMap = XmlGenerator::generateXmlMap(state);
    // auto processorNodeMap = pluginGraph->getAudioProcessorNodeMap();

    // std::map<std::string, NodeUIPtr> processorUIMap;
    
    // //Generate all processing UI
    // for (auto pair : xmlMap)
    // {
    //     auto id = pair.first;
    //     auto xml = pair.second;
    //     auto type = (NodeType)(xml->getChildByName(TYPE_TAG)->getAllSubText().getIntValue());
    //     auto x = xml->getChildByName(X_TAG)->getAllSubText().getIntValue();
    //     auto y = xml->getChildByName(Y_TAG)->getAllSubText().getIntValue();
    //     auto isReversed = xml->getChildByName(IS_REVERSED_TAG)->getAllSubText().getIntValue();
    //     auto parametersXML = xml->getChildByName(PARAMETERTS_TAG);

    //     NodeUIPtr processorUI = nullptr;

    //     if (type == NodeType::Input)
    //     {
    //         auto channel = parametersXML->getChildByName(IO_CHANNEL_TAG)->getAllSubText().getIntValue();
    //         auto processorNode = pluginGraph->getInputs()[channel];

    //         processorUI = processorNodeUIInteractionHandler.createNode(NodeType::Input, Point<int>(x, y), processorNode);
    //         processorUI->setAudioParametersFromXml(parametersXML);

    //         inputs.add(processorUI);
    //     }
    //     else if (type == NodeType::Output)
    //     {
    //         auto channel = parametersXML->getChildByName(IO_CHANNEL_TAG)->getAllSubText().getIntValue();
    //         auto processorNode = pluginGraph->getOutputs()[channel];

    //         processorUI = processorNodeUIInteractionHandler.createNode(NodeType::Output, Point<int>(), processorNode);
    //         processorUI->setTopLeftPosition(Point<int>(x, y));
    //         processorUI->setAudioParametersFromXml(parametersXML);

    //         outputs.add(processorUI);
    //     }
    //     else if (processorNodeMap.count(id) > 0)//Processor Exists
    //     {
    //         auto processorNode = processorNodeMap[id];
    //         processorUI = processorNodeUIInteractionHandler.createNode(type, Point<int>(x, y), processorNode);
    //     }
    //     else //Processor does not exist
    //     {
    //         processorUI = processorNodeUIInteractionHandler.createNode(type, Point<int>(x, y));
    //         processorUI->setAudioParametersFromXml(parametersXML);
    //     }

    //     if (isReversed)
    //         processorUI->reverse();

    //     processorNodeUIInteractionHandler.initializeProcessor(processorUI);
    //     processorUIMap[id] = processorUI;
    // }

    // createAllConnections(processorUIMap, xmlMap);

    // state->deleteAllChildElements();
}