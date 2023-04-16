/*
  ==============================================================================

    GraphEditor.cpp
    Created: 13 Sep 2020 2:49:28pm
    Author:  Will

  ==============================================================================
*/

#include <map>
#include <dsp/PluginGraph.h>
#include <ui/GraphEditor.h>
#include <ui/audioProcessorNodes/GainNode.h>
#include <ui/audioProcessorNodes/AudioProcessorNodeUIFactory.h>
#include <ui/interaction/NodeConnectionDrawer.h>
#include <utils/HitTest.h>
#include <utils/XmlGenerator.h>
#include <utils/StorageManager.h>
#include <utils/PresetFactory.h>

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
    pluginGraph->setPluginState(generatePluginState());
}

void GraphEditor::mouseDown(const MouseEvent& e)
{
    dragHandler.mouseDown(e);

    Array<AudioProcessorNodeUI*> tempProcessors;
    for (auto processor : processors)
    {
        tempProcessors.add(processor.get());
    }

    globalSelection.setItems(tempProcessors);

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
        NodeConnectionDrawer::drawConnection(g, this, connection->getStart(), connection->getEnd());
}

void GraphEditor::drawPotentialConnection(Graphics& g)
{
    if (interactionState == InteractionState::DragConnection)
    {
        g.setColour(Colour::fromString(CONNECTION_COLOUR));

        Point<int> pos = clickedNode->getScreenPosition();
        pos = getLocalPoint(nullptr, pos);

        auto processor = (AudioProcessorNodeUI*)(clickedNode->getParentComponent());
        auto isReversed = processor->isReversed();

        int x1 = pos.getX() + NODE_RADIUS;
        int y1 = pos.getY() + NODE_RADIUS;
        int x2 = mousePosition.getX();
        int y2 = mousePosition.getY();

        if (clickedNode->getType() == NodeType::AudioInput)
        {
            x1 = mousePosition.getX();
            y1 = mousePosition.getY();
            x2 = pos.getX();
            y2 = pos.getY();
        }

        NodeConnectionDrawer::drawRightAngleConnection(g, Point<int>(x1, y1), Point<int>(x2, y2));
    }
}

void GraphEditor::addNodeListeners(Array<Node*> nodes)
{
    for (auto node : nodes)
        node->addListener(this);
}

void GraphEditor::createIOProcessors()
{
    auto audioInputs = pluginGraph->getInputs();
    auto audioOutputs = pluginGraph->getOutputs();

    auto y = IO_START_Y;
    for (int i = 0; i < audioInputs.size(); i++)
    {
        auto audioInput = audioInputs[i];
        auto newInput = processorUIHandler.createDspObject(DspObjectType::Input, Point<int>(), audioInput);
        processorUIHandler.initializeProcessor(newInput);
        inputs.add(newInput);
        ((Input*)newInput.get())->setChannel(i);
        newInput->setTopLeftPosition(INPUT_START_X, y);
        y += IO_SPACING;
    }

    y = IO_START_Y;
    for (int i = 0; i < audioOutputs.size(); i++)
    {
        auto audioOutput = audioOutputs[i];
        auto newOutput = processorUIHandler.createDspObject(DspObjectType::Output, Point<int>(), audioOutput);
        processorUIHandler.initializeProcessor(newOutput);

        outputs.add(newOutput);
        ((Output*)newOutput.get())->setChannel(i);
        newOutput->setTopLeftPosition(OUTPUT_START_X, y);
        y += IO_SPACING;
    }
}

void GraphEditor::createAllConnections(std::map<std::string, AudioProcessorNodeUIPtr> processorUIMap, std::map<std::string, XmlElement*> xmlMap)
{
    for (auto pair : processorUIMap)
    {
        auto id = pair.first;
        auto processorUI = pair.second;

        auto xml = xmlMap[id];

        auto inputsXml = xml->getChildByName(INPUT_CONNECTIONS_TAG);
        if (inputsXml != nullptr)
        {
            auto inputXml = inputsXml->getFirstChildElement();
            while (inputXml != nullptr)
            {
                auto inputId = inputXml->getAllSubText().toStdString();
                auto inputProcessorUI = processorUIMap[inputId];

                auto startNode = inputProcessorUI->getOutputNode();
                auto endNode = processorUI->getInputNode();

                if (!connectionHandler.connectionExists(startNode, endNode))
                {
                    connections.add(std::make_shared<NodeConnection>(startNode, endNode));

                    processorUI->connectInput(inputProcessorUI.get());
                    inputProcessorUI->connectOutput(processorUI.get());
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
                auto outputProcessorUI = processorUIMap[outputId];

                auto startNode = processorUI->getOutputNode();
                auto endNode = outputProcessorUI->getInputNode();

                if (!connectionHandler.connectionExists(startNode, endNode))
                {
                    connections.add(std::make_shared<NodeConnection>(startNode, endNode));

                    processorUI->connectOutput(outputProcessorUI.get());
                    outputProcessorUI->connectInput(processorUI.get());
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
                auto feedbackProcessorUI = processorUIMap[feedbackId];

                auto startNode = feedbackProcessorUI->getOutputNode();
                auto endNode = processorUI->getInputNode();

                if (!connectionHandler.connectionExists(startNode, endNode))
                {
                    connections.add(std::make_shared<NodeConnection>(startNode, endNode));
                    processorUI->connectFeedbackInput(feedbackProcessorUI.get());
                }

                feedbackXml = feedbackXml->getNextElement();
            }
        }
    }
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
        auto processorUI = AudioProcessorNodeUIFactory::Generate(DspObjectType::Macro);
        processorUIHandler.initializeProcessor(processorUI);
    }
    else if (contextSelection > 0)
    {
        auto processorUI = processorUIHandler.createDspObject((DspObjectType)contextSelection, e.getPosition());
        processorUIHandler.initializeProcessor(processorUI);
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
    for (auto processor : selectionHandler.getItems())
    {
        if ((int)processor->getType() <= 0)
            continue;

        processorUIHandler.duplicateProcessor(processor);
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
    for (auto processor : selectionHandler.getItems())
    {
        if ((int)processor->getType() <= 0)
            continue;

        processorUIHandler.deleteProcessor(processor);
    }

    selectionHandler.clear();
}

Node* GraphEditor::getNodeAtPosition(Point<int> screenPos)
{
    for (auto* node : nodes)
    {
        auto nodePos = node->getScreenPosition();
        auto nodeRadius = node->getBounds().getWidth();

        if (HitTest::PointSphereHit(screenPos, nodePos, nodeRadius))
            return node;
    }

    return nullptr;
}

Array<AudioProcessorNodeUI*> GraphEditor::getOverlappingProcessors(Rectangle<int> bounds)
{
    Array<AudioProcessorNodeUI*> selectedProcessors;

    for (auto processor : processors)
    {
        auto otherBounds = processor->getBounds();
        if (HitTest::BoxHit(bounds, otherBounds))
            selectedProcessors.add(processor.get());
    }

    return selectedProcessors;
}

void GraphEditor::onNewProject()
{
    auto io = Array<AudioProcessorNodeUIPtr>();
    io.addArray(inputs);
    io.addArray(outputs);

    auto processorsToDelete = processors;
    processorsToDelete.removeValuesIn(io);
    processors.removeValuesNotIn(io);

    for (auto processor : processorsToDelete)
    {
        if (!io.contains(processor))
        {
            processorUIHandler.deleteProcessor(processor.get());
        }
    }

    repaint();
}

void GraphEditor::onPresetSelected(Presets presetId)
{
    clear();

    auto xmlString = PresetFactory::getPreset(presetId);
    auto callback = std::bind(&GraphEditor::loadStateFromFile, this, xmlString);
    
    auto state = loadStateFromFile(xmlString);
    pluginGraph->deleteAndReplaceAudioBlocks(state);
}

void GraphEditor::onSave(std::string savePath)
{
    auto state = generatePluginState();
    StorageManager::saveXML(savePath, state->toString().toStdString());
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
    const MessageManagerLock mmlock;

    auto xml = parseXML(xmlString);
    auto state = xml.get();

    if (state == nullptr || state->getTagName() != PLUGIN_STATE_TAG)
    {
        return nullptr;
    }
    
    auto tempInputs = Array<AudioProcessorNodePtr>();
    auto tempOutputs = Array<AudioProcessorNodePtr>();
    auto tempAllBlocks = Array<AudioProcessorNodePtr>();

    auto xmlMap = XmlGenerator::generateXmlMap(state);

    std::map<std::string, AudioProcessorNodeUIPtr> processorUIMap;

    //Generate all processing UI
    for (auto pair : xmlMap)
    {
        auto id = pair.first;
        auto xml = pair.second;
        auto type = (DspObjectType)(xml->getChildByName(TYPE_TAG)->getAllSubText().getIntValue());
        auto x = xml->getChildByName(X_TAG)->getAllSubText().getIntValue();
        auto y = xml->getChildByName(Y_TAG)->getAllSubText().getIntValue();
        auto isReversed = xml->getChildByName(IS_REVERSED_TAG)->getAllSubText().getIntValue();
        auto parametersXML = xml->getChildByName(PARAMETERTS_TAG);

        AudioProcessorNodeUIPtr processorUI;

        if (type == DspObjectType::Input)
        {
            auto channel = parametersXML->getChildByName(IO_CHANNEL_TAG)->getAllSubText().getIntValue();

            processorUI = processorUIHandler.createDspObject(DspObjectType::Input, Point<int>(x, y));
            processorUI->setAudioParametersFromXml(parametersXML);

            inputs.add(processorUI);
            tempInputs.add(processorUI->getProcessingBlock());
        }
        else if (type == DspObjectType::Output)
        {
            auto channel = parametersXML->getChildByName(IO_CHANNEL_TAG)->getAllSubText().getIntValue();

            processorUI = processorUIHandler.createDspObject(DspObjectType::Output, Point<int>(x, y));
            processorUI->setAudioParametersFromXml(parametersXML);

            outputs.add(processorUI);
            tempOutputs.add(processorUI->getProcessingBlock());
        }
        else //Processor does not exist
        {
            processorUI = processorUIHandler.createDspObject(type, Point<int>(x, y));
            processorUI->setAudioParametersFromXml(parametersXML);

            tempAllBlocks.add(processorUI->getProcessingBlock());
        }

        if (isReversed)
            processorUI->reverse();

        processorUIHandler.initializeProcessor(processorUI);

        processorUIMap[id] = processorUI;
    }

    createAllConnections(processorUIMap, xmlMap);

    xml->deleteAllChildElements();

    repaint();
    return std::make_unique<AudioProcessorState>(tempInputs, tempOutputs, tempAllBlocks);
}

void GraphEditor::clear()
{
    inputs.clear();
    outputs.clear();
    processors.clear();
    nodes.clear();
    connections.clear();
    globalSelection.clear();
    selectionHandler.clear();
}

void GraphEditor::onNodeLeftClick(Node* node, const MouseEvent& e)
{
    interactionState = InteractionState::DragConnection;
    clickedNode = node;
}

void GraphEditor::onNodeRightClick(Node* node, const MouseEvent& e)
{
    connectionHandler.deleteConnection(node);
}

void GraphEditor::onNodeDrag(Node* node, const MouseEvent& e)
{
    auto pos = e.getScreenPosition();
    mousePosition = getLocalPoint(nullptr, pos);

    repaint();
}

void GraphEditor::onNodeLeftRelease(Node* node, const MouseEvent& e)
{
    auto* mouseUpNode = getNodeAtPosition(e.getScreenPosition());

    if (mouseUpNode)
    {
        auto* startNode = clickedNode->getType() == NodeType::AudioOutput ? clickedNode : mouseUpNode;
        auto* endNode = mouseUpNode->getType() == NodeType::AudioInput ? mouseUpNode : clickedNode;

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

XmlElementPtr GraphEditor::generatePluginState()
{
    auto processorsToSave = processors;
    processorsToSave.removeValuesIn(inputs);
    processorsToSave.removeValuesIn(outputs);

    auto state = XmlGenerator::generatePluginState(inputs, outputs, processorsToSave);

    return state;
}

void GraphEditor::loadFromExistingState(XmlElement* state)
{
    //Load xml 
    if (state == nullptr || state->getTagName() != PLUGIN_STATE_TAG)
    {
        return;
    }
    
    auto xmlMap = XmlGenerator::generateXmlMap(state);
    auto processingBlockMap = pluginGraph->getAudioProcessorNodeMap();

    std::map<std::string, AudioProcessorNodeUIPtr> processorUIMap;
    
    //Generate all processing UI
    for (auto pair : xmlMap)
    {
        auto id = pair.first;
        auto xml = pair.second;
        auto type = (DspObjectType)(xml->getChildByName(TYPE_TAG)->getAllSubText().getIntValue());
        auto x = xml->getChildByName(X_TAG)->getAllSubText().getIntValue();
        auto y = xml->getChildByName(Y_TAG)->getAllSubText().getIntValue();
        auto isReversed = xml->getChildByName(IS_REVERSED_TAG)->getAllSubText().getIntValue();
        auto parametersXML = xml->getChildByName(PARAMETERTS_TAG);

        AudioProcessorNodeUIPtr processorUI = nullptr;

        if (type == DspObjectType::Input)
        {
            auto channel = parametersXML->getChildByName(IO_CHANNEL_TAG)->getAllSubText().getIntValue();
            auto processingBlock = pluginGraph->getInputs()[channel];

            processorUI = processorUIHandler.createDspObject(DspObjectType::Input, Point<int>(x, y), processingBlock);
            processorUI->setAudioParametersFromXml(parametersXML);

            inputs.add(processorUI);
        }
        else if (type == DspObjectType::Output)
        {
            auto channel = parametersXML->getChildByName(IO_CHANNEL_TAG)->getAllSubText().getIntValue();
            auto processingBlock = pluginGraph->getOutputs()[channel];

            processorUI = processorUIHandler.createDspObject(DspObjectType::Output, Point<int>(), processingBlock);
            processorUI->setTopLeftPosition(Point<int>(x, y));
            processorUI->setAudioParametersFromXml(parametersXML);

            outputs.add(processorUI);
        }
        else if (processingBlockMap.count(id) > 0)//Processor Exists
        {
            auto processingBlock = processingBlockMap[id];
            processorUI = processorUIHandler.createDspObject(type, Point<int>(x, y), processingBlock);
        }
        else //Processor does not exist
        {
            processorUI = processorUIHandler.createDspObject(type, Point<int>(x, y));
            processorUI->setAudioParametersFromXml(parametersXML);
        }

        if (isReversed)
            processorUI->reverse();

        processorUIHandler.initializeProcessor(processorUI);
        processorUIMap[id] = processorUI;
    }

    createAllConnections(processorUIMap, xmlMap);

    state->deleteAllChildElements();
}

void GraphEditor::onProcessorClicked(AudioProcessorNodeUI* processor, const MouseEvent& e)
{
    if (!selectionHandler.contains(processor))
    {
        selectionHandler.clear();
    }

    repaint();
}

void GraphEditor::onProcessorMoved(AudioProcessorNodeUI* processor, const MouseEvent& e)
{
    auto newE = e.getEventRelativeTo(this);
    selectionHandler.moveItems(newE.getOffsetFromDragStart());
   
    repaint();
}

void GraphEditor::onProcessorReleased(AudioProcessorNodeUI* processor, const MouseEvent& e)
{
    globalSelection.updateItemPositions();
    selectionHandler.updateItemPositions();
}

void GraphEditor::onContextSelection(AudioProcessorNodeUI* processor, AudioProcessorConextMenuItems selection)
{
    if (selection == AudioProcessorConextMenuItems::Delete)
    {
        if (!selectionHandler.isEmpty())
            deleteSelectedProcessors();
        else
            processorUIHandler.deleteProcessor(processor);
    }
    else if (selection == AudioProcessorConextMenuItems::Duplicate)
    {
        if (!selectionHandler.isEmpty())
            duplicateSelectedProcessors();
        else
            processorUIHandler.duplicateProcessor(processor);
    }
    else if (selection == AudioProcessorConextMenuItems::Reverse)
    {
        if (!selectionHandler.isEmpty())
            reverseSelectedProcessors();
        else
            processor->reverse();
    }

    repaint();
}
