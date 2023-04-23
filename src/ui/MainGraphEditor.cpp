#include <ui/MainGraphEditor.h>
#include <dsp/PluginGraph.h>
#include <utils/Constants.h>

MainGraphEditor::MainGraphEditor() : GraphEditor()
{
    mainMenu.setAlwaysOnTop(true);
    mainMenu.addListener(&mainMenuInteractionHandler);
    mainMenu.setBounds(0, 0, getWidth(), MENU_HEIGHT);
    addAndMakeVisible(mainMenu);
}

MainGraphEditor::~MainGraphEditor()
{
    // pluginGraph->setPluginState(generatePluginState());
}

void MainGraphEditor::resized()
{
    mainMenu.setBounds(0, 0, getWidth(), MENU_HEIGHT);
}

void MainGraphEditor::setPluginGraph(PluginGraph* pluginGraph)
{
    this->pluginGraph = pluginGraph;
    auto pluginState = pluginGraph->getPluginState();
    nodeInteractionHandler.initialize(this, pluginGraph);
    connectionHandler.initialize(this, pluginGraph);

    if (pluginState != nullptr)
        loadFromExistingState(pluginState.get());
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
        ((Input*)newInput.get())->setChannel(i);
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
        ((Output*)newOutput.get())->setChannel(i);
        newOutput->setTopLeftPosition(OUTPUT_START_X, y);
        y += IO_SPACING;
    }
}

void MainGraphEditor::createAllConnections(std::map<std::string, NodeUIPtr> processorUIMap, std::map<std::string, XmlElement*> xmlMap)
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