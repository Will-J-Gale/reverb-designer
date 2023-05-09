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
    //@TODO Fix this
    // pluginGraph->setPluginState(XmlUtils::generatePluginState(nodes));
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
        auto newInput = nodeInteractionHandler.createIONode(NodeInstance::Input, Point<int>(), audioInput, i);
        nodeInteractionHandler.initializeNode(newInput);
        inputs.add(newInput);
        newInput->setTopLeftPosition(INPUT_START_X, y);
        y += IO_SPACING;
    }

    y = IO_START_Y;
    for (int i = 0; i < audioOutputs.size(); i++)
    {
        auto audioOutput = audioOutputs[i];
        auto newOutput = nodeInteractionHandler.createIONode(NodeInstance::Output, Point<int>(), audioOutput, i);
        nodeInteractionHandler.initializeNode(newOutput);

        outputs.add(newOutput);
        newOutput->setTopLeftPosition(OUTPUT_START_X, y);
        y += IO_SPACING;
    }
}

void MainGraphEditor::loadFromExistingState(XmlElement* state)
{
    //Load xml 
    if (state == nullptr || state->getTagName() != PLUGIN_STATE_TAG)
    {
        return;
    }
    
    auto idToXmlMap = XmlUtils::generateIdToNodeXmlElementMap(state);
    auto idToNodeMap = pluginGraph->getAudioProcessorNodeMap();

    std::map<std::string, NodeUIPtr> processorUIMap;
    
    //Generate all processing UI
    for (auto pair : idToXmlMap)
    {
        auto id = pair.first;
        auto xml = pair.second;
        auto nodeClass = (NodeClass)(xml->getChildByName(NODE_CLASS_TAG)->getAllSubText().getIntValue());
        auto nodeInstance = (NodeInstance)(xml->getChildByName(INSTANCE_TAG)->getAllSubText().getIntValue());
        auto x = xml->getChildByName(X_TAG)->getAllSubText().getIntValue();
        auto y = xml->getChildByName(Y_TAG)->getAllSubText().getIntValue();
        auto isReversed = xml->getChildByName(IS_REVERSED_TAG)->getAllSubText().getIntValue();
        auto parametersXML = xml->getChildByName(AUDIO_PARAMETERTS_TAG);

        NodeUIPtr nodeUI = nullptr;

        if (idToNodeMap.count(id) > 0)//Processor Exists
        {
            auto processorNode = idToNodeMap[id];
            nodeUI = nodeInteractionHandler.createNode(nodeInstance, Point<int>(x, y), processorNode);
        }
        else //Processor does not exist
            nodeUI = nodeInteractionHandler.createNode(nodeInstance, Point<int>(x, y));

        if (isReversed)
            nodeUI->reverse();

        nodeInteractionHandler.initializeNode(nodeUI);
        processorUIMap[id] = nodeUI;
    }

    createAllConnections(processorUIMap, idToXmlMap);
    state->deleteAllChildElements();
}

void MainGraphEditor::fromXml(XmlElement* xml)
{
    const MessageManagerLock mmlock;
    clear();
    pluginGraph->clear();

    createIOProcessors();
    GraphEditor::fromXml(xml);

    pluginGraph->updateProcessPath();
    repaint();
}