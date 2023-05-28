#include <ui/GraphEditor.h>
#include <dsp/PluginGraph.h>
#include <ui/interaction/NodeConnectionDrawer.h>
#include <utils/HitTest.h>
#include <utils/XmlUtils.h>
#include <utils/StorageManager.h>
#include <utils/HitTest.h>
#include <ui/menus/InputModals.h>

GraphEditor::GraphEditor()
{
    setBounds(0, 0, PLUGIN_WIDTH, PLUGIN_HEIGHT);
    _zoomHandler.setOriginalBounds(getBounds());
}

GraphEditor::GraphEditor(PluginGraph* pluginGraph, AudioProcessorMacroNode* parent)
{
    setBounds(0, 0, PLUGIN_WIDTH, PLUGIN_HEIGHT);
    _zoomHandler.setOriginalBounds(getBounds());
    setPluginGraph(pluginGraph);
    _parent = parent;
}

GraphEditor::~GraphEditor()
{
}

void GraphEditor::mouseDown(const MouseEvent& e)
{
    Array<NodeUI*> tempNodes;
    for (auto node : _nodes)
    {
        tempNodes.add(node.get());
    }

    _globalSelection.setItems(tempNodes);

    if (e.mods.isRightButtonDown())
        handleRightClick(e);
    else if(e.mods.isLeftButtonDown())
        handleLeftClick(e);
}

void GraphEditor::mouseDrag(const MouseEvent& e)
{
    if (_interactionState == InteractionState::CreateSelection)
    {
        _mousePosition = e.getPosition();
        auto width = abs(_mouseDownPosition.x - _mousePosition.x);
        auto height = abs(_mouseDownPosition.y - _mousePosition.y);
        auto topLeft = Point<int>(fmin(_mousePosition.x, _mouseDownPosition.x), fmin(_mousePosition.y, _mouseDownPosition.y));
        
        _selectionArea.setBounds(topLeft.x, topLeft.y, width, height);
        _selectionHandler.setItems(HitTest::getOverlappingNodes(_selectionArea, _nodes));
    }
    else if (e.mods.isMiddleButtonDown())
        _globalSelection.moveItems(e.getOffsetFromDragStart());

    repaint();
}

void GraphEditor::mouseUp(const MouseEvent& e)
{
    _globalSelection.updateItemPositions();
    _interactionState = InteractionState::None;
    _selectionArea.setSize(0, 0);
    repaint();
}

void GraphEditor::mouseWheelMove(const MouseEvent& event, const MouseWheelDetails& wheel)
{
    if (wheel.deltaY > 0)
        _zoomHandler.zoomIn();
    else
        _zoomHandler.zoomOut();
}

void GraphEditor::paint(Graphics& g)
{
    g.fillAll(Colour::fromString(BACKGROUND_COLOUR_STRING));
    drawConnections(g);

    if (_interactionState == InteractionState::CreateSelection)
    {
        g.setColour(Colour::fromString(SELECTION_AREA_COLOUR));
        g.fillRect(_selectionArea);
    }

    if (_selectionHandler.size() > 0)
    {
        for (auto processor : _selectionHandler.getItems())
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
    _pluginGraph = pluginGraph;
    _nodeInteractionHandler.initialize(this, pluginGraph);
    _connectionHandler.initialize(this, pluginGraph);
}



void GraphEditor::drawConnections(Graphics& g)
{
    if (_clickedNodeConnector != nullptr)
        drawPotentialConnection(g);

    g.setColour(Colour::fromString(CONNECTION_COLOUR));

    for (auto connection : _connections)
        NodeConnectionDrawer::drawConnection(g, this, connection->getStartConnector(), connection->getEndConnector());
}

void GraphEditor::drawPotentialConnection(Graphics& g)
{
    if (_interactionState == InteractionState::DragConnection)
    {
        g.setColour(Colour::fromString(CONNECTION_COLOUR));

        Point<int> pos = _clickedNodeConnector->getScreenPosition();
        pos = getLocalPoint(nullptr, pos);

        int x1 = pos.getX() + CONNECTOR_RADIUS;
        int y1 = pos.getY() + CONNECTOR_RADIUS;
        int x2 = _mousePosition.getX();
        int y2 = _mousePosition.getY();

        if (_clickedNodeConnector->getType() == NodeConnectorType::AudioInput)
        {
            x1 = _mousePosition.getX();
            y1 = _mousePosition.getY();
            x2 = pos.getX();
            y2 = pos.getY();
        }

        NodeConnectionDrawer::drawRightAngleConnection(g, Point<int>(x1, y1), Point<int>(x2, y2));
    }
}

void GraphEditor::addNodeConnectorListeners(Array<NodeConnectorUI*> nodeConnectors)
{
    for (auto nodeConnector : nodeConnectors)
        nodeConnector->addListener(&_nodeConnectorInteractionHandler);
}

void GraphEditor::addInputNode()
{
    auto newInput = _nodeInteractionHandler.createIONode(NodeInstance::Input, Point<int>(), 0);
    _nodeInteractionHandler.initializeNode(newInput);
    _inputs.add(newInput);
    newInput->setTopLeftPosition(INPUT_START_X, IO_START_Y);
}

void GraphEditor::addOutputNode()
{
    auto newOutput = _nodeInteractionHandler.createIONode(NodeInstance::Output, Point<int>(), 0);
    _nodeInteractionHandler.initializeNode(newOutput);
    newOutput->setTopLeftPosition(OUTPUT_START_X, IO_START_Y);
    _outputs.add(newOutput);
}

void GraphEditor::handleRightClick(const MouseEvent& e)
{
    int contextSelection = _contextMenu.show();

    if (contextSelection == (int)GraphEditorContextMenuItems::Duplicate)
    {
        duplicateSelectedProcessors();
    }
    else if (contextSelection == (int)GraphEditorContextMenuItems::Macro)
    {
        auto macroName = InputModals::runTextInputModal(MACRO_MODAL_TITLE, MACRO_MODAL_MESSAGE, MACRO_DEFAULT_NAME);
        auto node = _nodeInteractionHandler.createMacroNode(e.getPosition(), macroName);
        _nodeInteractionHandler.initializeNode(node);
    }
    else if (contextSelection > 0)
    {
        auto node = _nodeInteractionHandler.createNode((NodeInstance)contextSelection, e.getPosition());
        _nodeInteractionHandler.initializeNode(node);
    }
}

void GraphEditor::handleLeftClick(const MouseEvent& e)
{
    _selectionHandler.clear();
    _mouseDownPosition = e.getPosition();
    _interactionState = InteractionState::CreateSelection; 
}

void GraphEditor::duplicateSelectedProcessors()
{
    for (auto node : _selectionHandler.getItems())
    {
        NodeInstance nodeInstance = node->getNodeInstance();
        if (nodeInstance == NodeInstance::Input || nodeInstance == NodeInstance::Output)
            continue;

        _nodeInteractionHandler.duplicateNode(node);
    }
}

void GraphEditor::reverseSelectedProcessors()
{
    for (auto processor : _selectionHandler.getItems())
    {
        if (arrayContains(_inputs, processor) || arrayContains(_outputs, processor))
            continue;

        processor->reverse();
    }
}

void GraphEditor::deleteSelectedProcessors()
{
    for (auto node : _selectionHandler.getItems())
    {
        NodeInstance nodeInstance = node->getNodeInstance();
        if (nodeInstance == NodeInstance::Input || nodeInstance == NodeInstance::Output)
            continue;

        _nodeInteractionHandler.deleteNode(node);
    }

    _selectionHandler.clear();
}

Array<NodeConnectorUI*>& GraphEditor::getNodeConnectors()
{
    return _nodeConnectors;
}
Array<NodeUIPtr>& GraphEditor::getNodes()
{
    return _nodes;
}

Array<NodeUIPtr>& GraphEditor::getInputs()
{
    return _inputs;
}


Array<NodeUIPtr>& GraphEditor::getOutputs()
{
    return _outputs;
}

void GraphEditor::clear()
{
    auto io = Array<NodeUIPtr>();
    io.addArray(_inputs);
    io.addArray(_outputs);

    for(NodeUIPtr node : _nodes)
    {
        node->clearConnections();
    }

    auto nodesToDelete = _nodes;
    nodesToDelete.removeValuesIn(io);
    _nodes.removeValuesNotIn(io);

    _nodeConnectors.clear();
    _connections.clear();
    _globalSelection.clear();
    _selectionHandler.clear();

    repaint();
}

XmlElement* GraphEditor::toXml()
{
    XmlElement* graphEditorState = new XmlElement(PLUGIN_STATE_TAG);
    std::map<NodeUIPtr, XmlElement*> xmlMap;

    for (auto node : _nodes)
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
        if((nodeInstance == NodeInstance::Input || nodeInstance == NodeInstance::Output) && _parent != nullptr)
            continue;

        auto inputConnections = node->getInputConnections();
        auto outputConnections = node->getOutputConnections();
        auto feedbackConnections = node->getFeedbackConnections();

        if (!inputConnections.isEmpty())
        {
            auto inputConnectionsXml = nodeXml->createNewChildElement(INPUT_CONNECTIONS_TAG);
            for (auto inputNodeUI : inputConnections)
            {
                //@TODO Don't cast, just override getIdAsString in IO class
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
                //@TODO Don't cast, just override getIdAsString in IO class
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
                //@TODO Don't cast, just override getIdAsString in IO class
                if(feedbackNodeUI->getNodeInstance() == NodeInstance::Input)
                    XmlUtils::createAndAddElement(ID_TAG, dynamic_cast<IO*>(feedbackNodeUI)->getParentId(), feedbackConnectionsXml);
                else
                    XmlUtils::createAndAddElement(ID_TAG, feedbackNodeUI->getIdAsString(), feedbackConnectionsXml);
            }
        }
    }

    return graphEditorState;
}

void GraphEditor::fromXml(XmlElement* xml, IdToAudioProcessorMap* idToProcessorMap)
{
    const MessageManagerLock mmlock;
    auto idToNodeXmlElementMap = XmlUtils::generateIdToNodeXmlElementMap(xml);
    std::map<std::string, NodeUIPtr> idToNodeUIMap;

    for (auto pair : idToNodeXmlElementMap)
    {
        auto id = pair.first;
        auto nodeXml = pair.second;
        std::string testName = nodeXml->getChildByName(NAME_TAG)->getAllSubText().toStdString();
        auto nodeInstance = (NodeInstance)(nodeXml->getChildByName(INSTANCE_TAG)->getAllSubText().getIntValue());
        auto x = nodeXml->getChildByName(X_TAG)->getAllSubText().getIntValue();
        auto y = nodeXml->getChildByName(Y_TAG)->getAllSubText().getIntValue();
        auto isReversed = nodeXml->getChildByName(IS_REVERSED_TAG)->getAllSubText().getIntValue();

        NodeUIPtr nodeUI;

        //@TODO All these branches now use XML so could make this more generic
        if(nodeInstance == NodeInstance::Macro)
        {
            nodeUI = _nodeInteractionHandler.createMacroNodeFromXml(nodeXml);
        }
        else if (nodeInstance == NodeInstance::Input || nodeInstance == NodeInstance::Output)
        {
            int channel = nodeXml->getChildByName(IO_CHANNEL_TAG)->getAllSubText().getIntValue();
            if(nodeInstance == NodeInstance::Input)
                nodeUI = _inputs[channel];
            else
                nodeUI = _outputs[channel];
            jassert(nodeUI != nullptr);
            nodeUI->fromXml(nodeXml);
        }
        else
        {
            if(idToProcessorMap != nullptr && idToProcessorMap->at(id) != nullptr)
                nodeUI = _nodeInteractionHandler.createNode(nodeInstance, Point<int>(x, y), idToProcessorMap->at(id));
            else
                nodeUI = _nodeInteractionHandler.createNode(nodeInstance, Point<int>(x, y));

            nodeUI->fromXml(nodeXml);
        }

        if(nodeInstance != NodeInstance::Input && nodeInstance != NodeInstance::Output)
            _nodeInteractionHandler.initializeNode(nodeUI);

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

                if (!_connectionHandler.connectionExists(startConnector, endConnector))
                {
                    _connections.add(std::make_shared<NodeConnection>(startConnector, endConnector));

                    //Get macro node input node if it's a macro
                    nodeUI = _connectionHandler.handleMacroNode(nodeUI, true);
                    inputNodeUI = _connectionHandler.handleMacroNode(inputNodeUI, false);

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

                if (!_connectionHandler.connectionExists(startConnector, endConnector))
                {
                    _connections.add(std::make_shared<NodeConnection>(startConnector, endConnector));

                    nodeUI = _connectionHandler.handleMacroNode(nodeUI, false);
                    outputNodeUI = _connectionHandler.handleMacroNode(outputNodeUI, true);

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

                nodeUI = _connectionHandler.handleMacroNode(nodeUI, true);
                feedbackNodeUI = _connectionHandler.handleMacroNode(feedbackNodeUI, false);

                if (!_connectionHandler.connectionExists(startConnector, endConnector))
                {
                    _connections.add(std::make_shared<NodeConnection>(startConnector, endConnector));
                    nodeUI->connectFeedbackInput(feedbackNodeUI);
                }

                feedbackXml = feedbackXml->getNextElement();
            }
        }
    }
}