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

    mainMenu.setAlwaysOnTop(true);
    addAndMakeVisible(mainMenu);
    mainMenu.addListener(this);
}

GraphEditor::~GraphEditor()
{
    // pluginGraph->setPluginState(generatePluginState());
}

void GraphEditor::mouseDown(const MouseEvent& e)
{
    dragHandler.mouseDown(e);

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
        selectionHandler.setItems(getOverlappingProcessors(selectionArea));
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
    auto pluginState = pluginGraph->getPluginState();
    processorNodeUIInteractionHandler.initialize(this, pluginGraph);
    connectionHandler.initialize(this, pluginGraph);

    if (pluginState != nullptr)
        loadFromExistingState(pluginState.get());
    else
        createIOProcessors();
}

void GraphEditor::resized()
{
    mainMenu.setBounds(0, 0, getWidth(), MENU_HEIGHT);
}

void GraphEditor::drawConnections(Graphics& g)
{
    if (clickedNode != nullptr)
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

        Point<int> pos = clickedNode->getScreenPosition();
        pos = getLocalPoint(nullptr, pos);

        auto processor = (NodeUI*)(clickedNode->getParentComponent());
        auto isReversed = processor->isReversed();

        int x1 = pos.getX() + NODE_RADIUS;
        int y1 = pos.getY() + NODE_RADIUS;
        int x2 = mousePosition.getX();
        int y2 = mousePosition.getY();

        if (clickedNode->getType() == NodeConnectorType::AudioInput)
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
        nodeConnector->addListener(this);
}

void GraphEditor::createIOProcessors()
{
    auto audioInputs = pluginGraph->getInputs();
    auto audioOutputs = pluginGraph->getOutputs();

    auto y = IO_START_Y;
    for (int i = 0; i < audioInputs.size(); i++)
    {
        auto audioInput = audioInputs[i];
        auto newInput = processorNodeUIInteractionHandler.createNode(NodeInstance::Input, Point<int>(), audioInput);
        processorNodeUIInteractionHandler.initializeProcessor(newInput);
        inputs.add(newInput);
        ((Input*)newInput.get())->setChannel(i);
        newInput->setTopLeftPosition(INPUT_START_X, y);
        y += IO_SPACING;
    }

    y = IO_START_Y;
    for (int i = 0; i < audioOutputs.size(); i++)
    {
        auto audioOutput = audioOutputs[i];
        auto newOutput = processorNodeUIInteractionHandler.createNode(NodeInstance::Output, Point<int>(), audioOutput);
        processorNodeUIInteractionHandler.initializeProcessor(newOutput);

        outputs.add(newOutput);
        ((Output*)newOutput.get())->setChannel(i);
        newOutput->setTopLeftPosition(OUTPUT_START_X, y);
        y += IO_SPACING;
    }
}

void GraphEditor::createAllConnections(std::map<std::string, NodeUIPtr> processorUIMap, std::map<std::string, XmlElement*> xmlMap)
{
    // for (auto pair : processorUIMap)
    // {
    //     auto id = pair.first;
    //     auto processorUI = pair.second;

    //     auto xml = xmlMap[id];

    //     auto inputsXml = xml->getChildByName(INPUT_CONNECTIONS_TAG);
    //     if (inputsXml != nullptr)
    //     {
    //         auto inputXml = inputsXml->getFirstChildElement();
    //         while (inputXml != nullptr)
    //         {
    //             auto inputId = inputXml->getAllSubText().toStdString();
    //             auto inputProcessorUI = processorUIMap[inputId];

    //             auto startNode = inputProcessorUI->getOutputNode();
    //             auto endNode = processorUI->getInputNode();

    //             if (!connectionHandler.connectionExists(startNode, endNode))
    //             {
    //                 connections.add(std::make_shared<NodeConnection>(startNode, endNode));

    //                 processorUI->connectInput(inputProcessorUI.get());
    //                 inputProcessorUI->connectOutput(processorUI.get());
    //             }

    //             inputXml = inputXml->getNextElement();
    //         }
    //     }

    //     auto outputsXml = xml->getChildByName(OUTPUT_CONNECTIONS_TAG);
    //     if (outputsXml != nullptr)
    //     {
    //         auto outputXml = outputsXml->getFirstChildElement();
    //         while (outputXml != nullptr)
    //         {
    //             auto outputId = outputXml->getAllSubText().toStdString();
    //             auto outputProcessorUI = processorUIMap[outputId];

    //             auto startNode = processorUI->getOutputNode();
    //             auto endNode = outputProcessorUI->getInputNode();

    //             if (!connectionHandler.connectionExists(startNode, endNode))
    //             {
    //                 connections.add(std::make_shared<NodeConnection>(startNode, endNode));

    //                 processorUI->connectOutput(outputProcessorUI.get());
    //                 outputProcessorUI->connectInput(processorUI.get());
    //             }

    //             outputXml = outputXml->getNextElement();
    //         }
    //     }

    //     auto feedbackInputsXml = xml->getChildByName(FEEDBACK_CONNECTIONS_TAG);
    //     if (feedbackInputsXml != nullptr)
    //     {
    //         auto feedbackXml = feedbackInputsXml->getFirstChildElement();
    //         while (feedbackXml != nullptr)
    //         {
    //             auto feedbackId = feedbackXml->getAllSubText().toStdString();
    //             auto feedbackProcessorUI = processorUIMap[feedbackId];

    //             auto startNode = feedbackProcessorUI->getOutputNode();
    //             auto endNode = processorUI->getInputNode();

    //             if (!connectionHandler.connectionExists(startNode, endNode))
    //             {
    //                 connections.add(std::make_shared<NodeConnection>(startNode, endNode));
    //                 processorUI->connectFeedbackInput(feedbackProcessorUI.get());
    //             }

    //             feedbackXml = feedbackXml->getNextElement();
    //         }
    //     }
    // }
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
        // auto processorUI = processorNodeUIInteractionHandler.createNodeUI((NodeType)contextSelection, e.getPosition());
        // processorNodeUIInteractionHandler.initializeProcessor(processorUI);
    }
    else if (contextSelection > 0)
    {
        auto processorUI = processorNodeUIInteractionHandler.createNode((NodeInstance)contextSelection, e.getPosition());
        processorNodeUIInteractionHandler.initializeProcessor(processorUI);
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

        processorNodeUIInteractionHandler.duplicateProcessor(node);
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

        processorNodeUIInteractionHandler.deleteProcessor(node);
    }

    selectionHandler.clear();
}

NodeConnectorUI* GraphEditor::getNodeConnectorAtPosition(Point<int> screenPos)
{
    for (auto* nodeConnector : nodeConnectors)
    {
        auto nodePos = nodeConnector->getScreenPosition();
        auto nodeRadius = nodeConnector->getBounds().getWidth();
        
        if (HitTest::PointSphereHit(screenPos, nodePos, nodeRadius))
            return nodeConnector;
    }

    return nullptr;
}

Array<NodeUI*> GraphEditor::getOverlappingProcessors(Rectangle<int> bounds)
{
    Array<NodeUI*> selectedProcessors;

    for (auto node : nodes)
    {
        auto otherBounds = node->getBounds();
        if (HitTest::BoxHit(bounds, otherBounds))
            selectedProcessors.add(node.get());
    }

    return selectedProcessors;
}

void GraphEditor::onNewProject()
{
    auto io = Array<NodeUIPtr>();
    io.addArray(inputs);
    io.addArray(outputs);

    auto nodesToDelete = nodes;
    nodesToDelete.removeValuesIn(io);
    nodes.removeValuesNotIn(io);

    for (auto node : nodesToDelete)
    {
        if (!io.contains(node))
        {
            processorNodeUIInteractionHandler.deleteProcessor(node.get());
        }
    }

    repaint();
}

void GraphEditor::onPresetSelected(Presets presetId)
{
    // clear();

    // auto xmlString = PresetFactory::getPreset(presetId);
    // auto callback = std::bind(&GraphEditor::loadStateFromFile, this, xmlString);
    
    // auto state = loadStateFromFile(xmlString);
    // pluginGraph->deleteAndReplaceAudioBlocks(state);
}

void GraphEditor::onSave(std::string savePath)
{
    // auto state = generatePluginState();
    // StorageManager::saveXML(savePath, state->toString().toStdString());
}

void GraphEditor::onLoad(std::string filepath)
{
    clear();

    auto file = File(filepath);
    auto xmlString = file.loadFileAsString().toStdString();
    auto callback = std::bind(&GraphEditor::loadStateFromFile, this, xmlString);
    
    pluginGraph->deleteAndReplaceAudioBlocks(callback);
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

void GraphEditor::onNodeConnectorLeftClick(NodeConnectorUI* node, const MouseEvent& e)
{
    interactionState = InteractionState::DragConnection;
    clickedNode = node;
}

void GraphEditor::onNodeConnectorRightClick(NodeConnectorUI* node, const MouseEvent& e)
{
    connectionHandler.deleteConnection(node);
}

void GraphEditor::onNodeConnectorDrag(NodeConnectorUI* nodeConnector, const MouseEvent& e)
{
    auto pos = e.getScreenPosition();
    mousePosition = getLocalPoint(nullptr, pos);

    repaint();
}

void GraphEditor::onNodeConnectorLeftRelease(NodeConnectorUI* nodeConnecctor, const MouseEvent& e)
{
    auto* mouseUpNodeConnector = getNodeConnectorAtPosition(e.getScreenPosition());

    if (mouseUpNodeConnector)
    {
        auto* startNode = clickedNode->getType() == NodeConnectorType::AudioOutput ? clickedNode : mouseUpNodeConnector;
        auto* endNode = mouseUpNodeConnector->getType() == NodeConnectorType::AudioInput ? mouseUpNodeConnector : clickedNode;

        if (!connectionHandler.connectionExists(startNode, endNode) 
            && connectionHandler.nodesAreCompatible(startNode, endNode))
        {
            if (connectionHandler.isCreatingFeedback(startNode, endNode))
                connectionHandler.createFeedbackConnection(startNode, endNode);
            else
                connectionHandler.createConnection(startNode, endNode);
        }
    }

    clickedNode = nullptr;
    interactionState = InteractionState::None;

    repaint();
}

// XmlElementPtr GraphEditor::generatePluginState()
// {
//     auto processorsToSave = nodes;
//     processorsToSave.removeValuesIn(inputs);
//     processorsToSave.removeValuesIn(outputs);

//     auto state = XmlGenerator::generatePluginState(inputs, outputs, processorsToSave);

//     return state;
// }

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

void GraphEditor::onNodeClicked(NodeUI* node, const MouseEvent& e)
{
    if (!selectionHandler.contains(node))
    {
        selectionHandler.clear();
    }

    repaint();
}

void GraphEditor::onNodeMoved(NodeUI* node, const MouseEvent& e)
{
    auto newE = e.getEventRelativeTo(this);
    selectionHandler.moveItems(newE.getOffsetFromDragStart());
   
    repaint();
}

void GraphEditor::onNodeReleased(NodeUI* node, const MouseEvent& e)
{
    globalSelection.updateItemPositions();
    selectionHandler.updateItemPositions();
}

void GraphEditor::onNodeContextSelection(NodeUI* node, NodeUIConextMenuItems selection)
{
    if (selection == NodeUIConextMenuItems::Delete)
    {
        if (!selectionHandler.isEmpty())
            deleteSelectedProcessors();
        else
            processorNodeUIInteractionHandler.deleteProcessor(node);
    }
    else if (selection == NodeUIConextMenuItems::Duplicate)
    {
        if (!selectionHandler.isEmpty())
            duplicateSelectedProcessors();
        else
            processorNodeUIInteractionHandler.duplicateProcessor(node);
    }
    else if (selection == NodeUIConextMenuItems::Reverse)
    {
        if (!selectionHandler.isEmpty())
            reverseSelectedProcessors();
        else
            node->reverse();
    }

    repaint();
}