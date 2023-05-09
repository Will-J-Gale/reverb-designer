#include <map>
#include <ui/GraphEditor.h>
#include <dsp/PluginGraph.h>
#include <ui/interaction/NodeConnectionDrawer.h>
#include <utils/HitTest.h>
#include <utils/XmlUtils.h>
#include <utils/StorageManager.h>
#include <utils/PresetFactory.h>
#include <utils/HitTest.h>
#include <ui/menus/InputModals.h>

GraphEditor::GraphEditor()
{
    setBounds(0, 0, PLUGIN_WIDTH, PLUGIN_HEIGHT);
    zoomHandler.setOriginalBounds(getBounds());
}

GraphEditor::GraphEditor(PluginGraph* pluginGraph, AudioProcessorMacroNode* parent)
{
    setBounds(0, 0, PLUGIN_WIDTH, PLUGIN_HEIGHT);
    zoomHandler.setOriginalBounds(getBounds());
    setPluginGraph(pluginGraph);
    this->parent = parent;
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

        int x1 = pos.getX() + CONNECTOR_RADIUS;
        int y1 = pos.getY() + CONNECTOR_RADIUS;
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
    auto newInput = nodeInteractionHandler.createIONode(NodeInstance::Input, Point<int>(), 0);
    nodeInteractionHandler.initializeNode(newInput);
    inputs.add(newInput);
    newInput->setTopLeftPosition(INPUT_START_X, IO_START_Y);
}

void GraphEditor::addOutputNode()
{
    auto newOutput = nodeInteractionHandler.createIONode(NodeInstance::Output, Point<int>(), 0);
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
        auto macroName = InputModals::runTextInputModal(MACRO_MODAL_TITLE, MACRO_MODAL_MESSAGE, MACRO_DEFAULT_NAME);
        auto node = nodeInteractionHandler.createMacroNode(e.getPosition(), macroName);
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

XmlElement* GraphEditor::toXml()
{
    XmlElement* graphEditorState = new XmlElement(PLUGIN_STATE_TAG);
    std::map<NodeUIPtr, XmlElement*> xmlMap;

    for (auto node : nodes)
    {
        XmlElement* nodeXml = node->toXml();
        graphEditorState->addChildElement(nodeXml);
        xmlMap[node] = nodeXml;
    }

    for (auto pair : xmlMap)
    {
        auto node = pair.first;
        auto nodeXml = pair.second;
        auto nodeInstance = node->getNodeInstance();

        //If graph editor is for Macro, ignore input/output connections as they are handled by parent
        if((nodeInstance == NodeInstance::Input || nodeInstance == NodeInstance::Output) && parent != nullptr)
            continue;

        auto inputConnections = node->getInputConnections();
        auto outputConnections = node->getOutputConnections();
        auto feedbackConnections = node->getFeedbackConnections();

        if (!inputConnections.isEmpty())
        {
            auto inputConnectionsXml = nodeXml->createNewChildElement(INPUT_CONNECTIONS_TAG);
            for (auto inputNodeUI : inputConnections)
            {
                if(inputNodeUI->getNodeInstance() == NodeInstance::Output)
                    XmlUtils::createAndAddElement(ID_TAG, dynamic_cast<IO*>(inputNodeUI)->getParentId(), inputConnectionsXml);
                else
                    XmlUtils::createAndAddElement(ID_TAG, inputNodeUI->getIdAsString(), inputConnectionsXml);
            }
        }
        
        if (!outputConnections.isEmpty())
        {
            auto outputConnectionsXml = nodeXml->createNewChildElement(OUTPUT_CONNECTIONS_TAG);
            for (auto outputNodeUI : outputConnections)
            {
                if(outputNodeUI->getNodeInstance() == NodeInstance::Input)
                    XmlUtils::createAndAddElement(ID_TAG, dynamic_cast<IO*>(outputNodeUI)->getParentId(), outputConnectionsXml);
                else
                    XmlUtils::createAndAddElement(ID_TAG, outputNodeUI->getIdAsString(), outputConnectionsXml);
            }
        }

        if (!feedbackConnections.isEmpty())
        {
            auto feedbackConnectionsXml = nodeXml->createNewChildElement(FEEDBACK_CONNECTIONS_TAG);
            for (auto feedbackNodeUI : feedbackConnections)
            {
                if(feedbackNodeUI->getNodeInstance() == NodeInstance::Input)
                    XmlUtils::createAndAddElement(ID_TAG, dynamic_cast<IO*>(feedbackNodeUI)->getParentId(), feedbackConnectionsXml);
                else
                    XmlUtils::createAndAddElement(ID_TAG, feedbackNodeUI->getIdAsString(), feedbackConnectionsXml);
            }
        }
    }

    return graphEditorState;
}

void GraphEditor::fromXml(XmlElement* xml)
{
    const MessageManagerLock mmlock;
    auto idToNodeXmlElementMap = XmlUtils::generateIdToNodeXmlElementMap(xml);
    std::map<std::string, NodeUIPtr> idToNodeUIMap;

    for (auto pair : idToNodeXmlElementMap)
    {
        auto id = pair.first;
        auto nodeXml = pair.second;
        std::string testName = nodeXml->getChildByName(NAME_TAG)->getAllSubText().toStdString();
        auto nodeClass = (NodeClass)(nodeXml->getChildByName(NODE_CLASS_TAG)->getAllSubText().getIntValue());
        auto nodeInstance = (NodeInstance)(nodeXml->getChildByName(INSTANCE_TAG)->getAllSubText().getIntValue());
        auto x = nodeXml->getChildByName(X_TAG)->getAllSubText().getIntValue();
        auto y = nodeXml->getChildByName(Y_TAG)->getAllSubText().getIntValue();
        auto isReversed = nodeXml->getChildByName(IS_REVERSED_TAG)->getAllSubText().getIntValue();
        auto parametersXML = nodeXml->getChildByName(AUDIO_PARAMETERTS_TAG);

        NodeUIPtr nodeUI;
        if(nodeInstance == NodeInstance::Macro)
        {
            nodeUI = nodeInteractionHandler.createMacroNode(Point<int>(x, y), std::string());
            nodeUI->fromXml(nodeXml);
        }
        else if (nodeInstance == NodeInstance::Input || nodeInstance == NodeInstance::Output)
        {
            int channel = nodeXml->getChildByName(IO_CHANNEL_TAG)->getAllSubText().getIntValue();
            if(nodeInstance == NodeInstance::Input)
                nodeUI = inputs[channel];
            else
                nodeUI = outputs[channel];
            jassert(nodeUI != nullptr);
            // nodeUI = nodeInteractionHandler.createIONode(nodeInstance, Point<int>(x, y), 0);
            // nodeUI->fromXml(nodeXml);

            // if(nodeInstance == NodeInstance::Input)
            //     inputs.add(nodeUI);
            // else
            //     outputs.add(nodeUI);
        }
        else
        {
            nodeUI = nodeInteractionHandler.createNode(nodeInstance, Point<int>(x, y));
            auto audioNodeUI = dynamic_cast<AudioProcessorNodeUI*>(nodeUI.get());
            auto audioProcessorNode = audioNodeUI->getProcessorNode();
            auto audioParametesXml = nodeXml->getChildByName(AUDIO_PARAMETERTS_TAG);
            XmlUtils::setAudioParametersFromXml(audioNodeUI->getAudioParameters(), audioParametesXml);
            audioNodeUI->updateParametersUI();

            // if(nodeInstance == NodeInstance::Input)
            //     new_input_nodes.add(audioProcessorNode);
            // else if(nodeInstance == NodeInstance::Output)
            //     new_output_nodes.add(audioProcessorNode);
            // else
            //     new_nodes.add(audioProcessorNode);
        }

        if(isReversed)
            nodeUI->reverse();
        
        if(nodeInstance != NodeInstance::Input && nodeInstance != NodeInstance::Output)
            nodeInteractionHandler.initializeNode(nodeUI);

        idToNodeUIMap[id] = nodeUI;
    }

    createAllConnections(idToNodeUIMap, idToNodeXmlElementMap);
    repaint();
}

void GraphEditor::createAllConnections(std::map<std::string, NodeUIPtr> idToNodeUIMap, std::map<std::string, XmlElement*> idToXmlElement)
{
    for (auto pair : idToNodeUIMap)
    {
        auto id = pair.first;
        auto nodeUI = pair.second.get();
        auto xml = idToXmlElement[id];

        auto inputsXml = xml->getChildByName(INPUT_CONNECTIONS_TAG);
        if (inputsXml != nullptr)
        {
            auto inputXml = inputsXml->getFirstChildElement();
            while (inputXml != nullptr)
            {
                auto inputId = inputXml->getAllSubText().toStdString();
                auto inputNodeUI = idToNodeUIMap[inputId].get();
                auto startConnector = inputNodeUI->getOutputConnector();
                auto endConnector = nodeUI->getInputConnector();

                if (!connectionHandler.connectionExists(startConnector, endConnector))
                {
                    connections.add(std::make_shared<NodeConnection>(startConnector, endConnector));

                    //Get macro node input node if it's a macro
                    nodeUI = connectionHandler.handleMacroNode(nodeUI, true);
                    inputNodeUI = connectionHandler.handleMacroNode(inputNodeUI, false);

                    nodeUI->connectInput(inputNodeUI);
                    inputNodeUI->connectOutput(nodeUI);
                }

                inputXml = inputXml->getNextElement();
            }
        }

        auto outputsXml = xml->getChildByName(OUTPUT_CONNECTIONS_TAG);
        if (outputsXml != nullptr)
        {
            auto outputXml = outputsXml->getFirstChildElement();
            while (outputXml != nullptr)
            {
                auto outputId = outputXml->getAllSubText().toStdString();
                auto outputNodeUI = idToNodeUIMap[outputId].get();
                auto startConnector = nodeUI->getOutputConnector();
                auto endConnector = outputNodeUI->getInputConnector();

                if (!connectionHandler.connectionExists(startConnector, endConnector))
                {
                    connections.add(std::make_shared<NodeConnection>(startConnector, endConnector));

                    nodeUI = connectionHandler.handleMacroNode(nodeUI, false);
                    outputNodeUI = connectionHandler.handleMacroNode(outputNodeUI, true);

                    nodeUI->connectOutput(outputNodeUI);
                    outputNodeUI->connectInput(nodeUI);
                }

                outputXml = outputXml->getNextElement();
            }
        }

        auto feedbackInputsXml = xml->getChildByName(FEEDBACK_CONNECTIONS_TAG);
        if (feedbackInputsXml != nullptr)
        {
            auto feedbackXml = feedbackInputsXml->getFirstChildElement();
            while (feedbackXml != nullptr)
            {
                auto feedbackId = feedbackXml->getAllSubText().toStdString();
                auto feedbackNodeUI = idToNodeUIMap[feedbackId].get();
                auto startConnector = feedbackNodeUI->getOutputConnector();
                auto endConnector = nodeUI->getInputConnector();

                nodeUI = connectionHandler.handleMacroNode(nodeUI, true);
                feedbackNodeUI = connectionHandler.handleMacroNode(feedbackNodeUI, false);

                if (!connectionHandler.connectionExists(startConnector, endConnector))
                {
                    connections.add(std::make_shared<NodeConnection>(startConnector, endConnector));
                    nodeUI->connectFeedbackInput(feedbackNodeUI);
                }

                feedbackXml = feedbackXml->getNextElement();
            }
        }
    }
}