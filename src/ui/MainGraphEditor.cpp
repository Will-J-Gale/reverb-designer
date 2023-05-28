#include <ui/MainGraphEditor.h>
#include <dsp/PluginGraph.h>
#include <utils/Constants.h>
#include <utils/XmlUtils.h>

MainGraphEditor::MainGraphEditor() : GraphEditor()
{
    _mainMenu.setAlwaysOnTop(true);
    _mainMenu.addListener(&_mainMenuInteractionHandler);
    _mainMenu.setBounds(0, 0, getWidth(), MENU_HEIGHT);
    addAndMakeVisible(_mainMenu);
}

MainGraphEditor::~MainGraphEditor()
{
    _pluginGraph->setPluginState(std::shared_ptr<XmlElement>(toXml()));
}

void MainGraphEditor::resized()
{
    _mainMenu.setBounds(0, 0, getWidth(), MENU_HEIGHT);
}

void MainGraphEditor::setPluginGraph(PluginGraph* pluginGraph)
{
    _pluginGraph = pluginGraph;
    auto pluginState = pluginGraph->getPluginState().get();
    _nodeInteractionHandler.initialize(this, pluginGraph);
    _connectionHandler.initialize(this, pluginGraph);


    if(_inputs.size() == 0 && _outputs.size() == 0)
        createIOProcessors();

    if (pluginState != nullptr)
        loadFromExistingState(pluginState);
}

void MainGraphEditor::createIOProcessors()
{
    auto audioInputs = _pluginGraph->getInputs();
    auto audioOutputs = _pluginGraph->getOutputs();

    auto y = IO_START_Y;
    for (int i = 0; i < audioInputs.size(); i++)
    {
        auto audioInput = audioInputs[i];
        auto newInput = _nodeInteractionHandler.createIONode(NodeInstance::Input, Point<int>(), audioInput, i);
        _nodeInteractionHandler.initializeNode(newInput);
        _inputs.add(newInput);
        newInput->setTopLeftPosition(INPUT_START_X, y);
        y += IO_SPACING;
    }

    y = IO_START_Y;
    for (int i = 0; i < audioOutputs.size(); i++)
    {
        auto audioOutput = audioOutputs[i];
        auto newOutput = _nodeInteractionHandler.createIONode(NodeInstance::Output, Point<int>(), audioOutput, i);
        _nodeInteractionHandler.initializeNode(newOutput);

        _outputs.add(newOutput);
        newOutput->setTopLeftPosition(OUTPUT_START_X, y);
        y += IO_SPACING;
    }
}

void MainGraphEditor::loadFromExistingState(XmlElement* state)
{
    //Load xml 
    if (state == nullptr || state->getTagName() != PLUGIN_STATE_TAG)
        return;

    auto idToXmlMap = XmlUtils::generateIdToNodeXmlElementMap(state);
    auto idToNodeMap = _pluginGraph->getAudioProcessorNodeMap();

    fromXml(state, &idToNodeMap);
    state->deleteAllChildElements();
}

void MainGraphEditor::fromXml(XmlElement* xml, IdToAudioProcessorMap* idToProcessorMap)
{
    const MessageManagerLock mmlock;
    clear();

    GraphEditor::fromXml(xml, idToProcessorMap);
    _pluginGraph->addAction(PluginGraphActionType::CalculateProcessPath);
    repaint();
}