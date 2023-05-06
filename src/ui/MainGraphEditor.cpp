#include <ui/MainGraphEditor.h>
#include <dsp/PluginGraph.h>
#include <utils/Constants.h>
#include <utils/XmlUtils.h>

MainGraphEditor::MainGraphEditor() : GraphEditor()
{
    mainMenu.setAlwaysOnTop(true);
    mainMenu.addListener(&mainMenuInteractionHandler);
    mainMenu.setBounds(0, 0, getWidth(), MENU_HEIGHT);
    addAndMakeVisible(mainMenu);
}

MainGraphEditor::~MainGraphEditor()
{
    pluginGraph->setPluginState(XmlUtils::generatePluginState(nodes));
}

void MainGraphEditor::resized()
{
    mainMenu.setBounds(0, 0, getWidth(), MENU_HEIGHT);
}

void MainGraphEditor::setPluginGraph(PluginGraph* pluginGraph)
{
    this->pluginGraph = pluginGraph;
    auto pluginState = pluginGraph->getPluginState().get();
    nodeInteractionHandler.initialize(this, pluginGraph);
    connectionHandler.initialize(this, pluginGraph);

    if (pluginState != nullptr)
        loadFromExistingState(pluginState);
    else
        createIOProcessors();
}

void MainGraphEditor::createIOProcessors()
{
    auto audioInputs = pluginGraph->getInputs();
    auto audioOutputs = pluginGraph->getOutputs();

    auto y = IO_START_Y;
    for (int i = 0; i < audioInputs.size(); i++)
    {
        auto audioInput = audioInputs[i];
        auto newInput = nodeInteractionHandler.createNode(NodeInstance::Input, Point<int>(), audioInput);
        nodeInteractionHandler.initializeNode(newInput);
        inputs.add(newInput);
        newInput->setTopLeftPosition(INPUT_START_X, y);
        y += IO_SPACING;
    }

    y = IO_START_Y;
    for (int i = 0; i < audioOutputs.size(); i++)
    {
        auto audioOutput = audioOutputs[i];
        auto newOutput = nodeInteractionHandler.createNode(NodeInstance::Output, Point<int>(), audioOutput);
        nodeInteractionHandler.initializeNode(newOutput);

        outputs.add(newOutput);
        newOutput->setTopLeftPosition(OUTPUT_START_X, y);
        y += IO_SPACING;
    }
}

void MainGraphEditor::createAllConnections(std::map<std::string, NodeUIPtr> idToNodeUIMap, std::map<std::string, XmlElement*> idToXmlElement)
{
    for (auto pair : idToNodeUIMap)
    {
        auto id = pair.first;
        auto nodeUI = pair.second;
        auto xml = idToXmlElement[id];

        auto inputsXml = xml->getChildByName(INPUT_CONNECTIONS_TAG);
        if (inputsXml != nullptr)
        {
            auto inputXml = inputsXml->getFirstChildElement();
            while (inputXml != nullptr)
            {
                auto inputId = inputXml->getAllSubText().toStdString();
                auto inputNodeUI = idToNodeUIMap[inputId];
                auto startConnector = inputNodeUI->getOutputConnector();
                auto endConnector = nodeUI->getInputConnector();

                if (!connectionHandler.connectionExists(startConnector, endConnector))
                {
                    connections.add(std::make_shared<NodeConnection>(startConnector, endConnector));

                    nodeUI->connectInput(inputNodeUI.get());
                    inputNodeUI->connectOutput(nodeUI.get());
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
                auto outputNodeUI = idToNodeUIMap[outputId];
                auto startConnector = nodeUI->getOutputConnector();
                auto endConnector = outputNodeUI->getInputConnector();

                if (!connectionHandler.connectionExists(startConnector, endConnector))
                {
                    connections.add(std::make_shared<NodeConnection>(startConnector, endConnector));

                    nodeUI->connectOutput(outputNodeUI.get());
                    outputNodeUI->connectInput(nodeUI.get());
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
                auto feedbackProcessorUI = idToNodeUIMap[feedbackId];
                auto startConnector = feedbackProcessorUI->getOutputConnector();
                auto endConnector = nodeUI->getInputConnector();

                if (!connectionHandler.connectionExists(startConnector, endConnector))
                {
                    connections.add(std::make_shared<NodeConnection>(startConnector, endConnector));
                    nodeUI->connectFeedbackInput(feedbackProcessorUI.get());
                }

                feedbackXml = feedbackXml->getNextElement();
            }
        }
    }
}

std::shared_ptr<AudioProcessorState> MainGraphEditor::loadStateFromFile(std::string xmlString)
{
    //This function is called from the audio processing thread so mmLock locks the UI thread so we can update the UI 
    const MessageManagerLock mmlock;

    auto state = parseXML(xmlString);

    if (state == nullptr || state->getTagName() != PLUGIN_STATE_TAG)
    {
        return nullptr;
    }
    
    auto new_input_nodes = Array<AudioProcessorNodePtr>();
    auto new_output_nodes = Array<AudioProcessorNodePtr>();
    auto new_nodes = Array<AudioProcessorNodePtr>();
    auto idToNodeXmlElementMap = XmlUtils::generateIdToNodeXmlElementMap(state.get());
    std::map<std::string, NodeUIPtr> idToNodeUIMap;

    //Generate all processing UI
    for (auto pair : idToNodeXmlElementMap)
    {
        auto id = pair.first;
        auto nodeXml = pair.second;
        auto nodeClass = (NodeClass)(nodeXml->getChildByName(NODE_CLASS_TAG)->getAllSubText().getIntValue());
        auto nodeInstance = (NodeInstance)(nodeXml->getChildByName(INSTANCE_TAG)->getAllSubText().getIntValue());
        auto x = nodeXml->getChildByName(X_TAG)->getAllSubText().getIntValue();
        auto y = nodeXml->getChildByName(Y_TAG)->getAllSubText().getIntValue();
        auto isReversed = nodeXml->getChildByName(IS_REVERSED_TAG)->getAllSubText().getIntValue();
        auto parametersXML = nodeXml->getChildByName(AUDIO_PARAMETERTS_TAG);

        NodeUIPtr nodeUI = nodeInteractionHandler.createNode(nodeInstance, Point<int>(x, y));
        nodeInteractionHandler.initializeNode(nodeUI);
        idToNodeUIMap[id] = nodeUI;

        if(nodeClass == NodeClass::AudioProcessor)
        {
            auto audioNodeUI = static_cast<AudioProcessorNodeUI*>(nodeUI.get());
            auto audioProcessorNode = audioNodeUI->getProcessorNode();
            auto audioParametesXml = nodeXml->getChildByName(AUDIO_PARAMETERTS_TAG);
            XmlUtils::setAudioParametersFromXml(audioNodeUI->getAudioParameters(), audioParametesXml);
            audioNodeUI->updateParametersUI();

            if(nodeInstance == NodeInstance::Input)
                new_input_nodes.add(audioProcessorNode);
            else if(nodeInstance == NodeInstance::Output)
                new_output_nodes.add(audioProcessorNode);
            else
                new_nodes.add(audioProcessorNode);
        }

        if(isReversed)
            nodeUI->reverse();
    }

    createAllConnections(idToNodeUIMap, idToNodeXmlElementMap);
    state->deleteAllChildElements();

    repaint();
    return std::make_unique<AudioProcessorState>(new_input_nodes, new_output_nodes, new_nodes);
}

void MainGraphEditor::loadFromExistingState(XmlElement* state)
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