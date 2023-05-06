#include <ui/interaction/MainMenuInteractionHandler.h>
#include <ui/MainGraphEditor.h>
#include <utils/XmlUtils.h>
#include <utils/StorageManager.h>
#include <dsp/PluginGraph.h>

MainMenuInteractionHandler::MainMenuInteractionHandler(MainGraphEditor* mainGraphEditor)
{
    this->mainGraphEditor = mainGraphEditor;
}

void MainMenuInteractionHandler::onNewProject()
{
    auto io = Array<NodeUIPtr>();
    io.addArray(mainGraphEditor->inputs);
    io.addArray(mainGraphEditor->outputs);

    auto nodesToDelete = mainGraphEditor->nodes;
    nodesToDelete.removeValuesIn(io);
    mainGraphEditor->nodes.removeValuesNotIn(io);

    for (auto node : nodesToDelete)
    {
        if (!io.contains(node))
        {
            mainGraphEditor->nodeInteractionHandler.deleteProcessor(node.get());
        }
    }

    mainGraphEditor->repaint();
}

void MainMenuInteractionHandler::onPresetSelected(PresetType presetId)
{
    // graphEditor->clear();

    // auto xmlString = PresetFactory::getPreset(presetId);
    // auto callback = std::bind(&GraphEditor::loadStateFromFile, this, xmlString);
    
    // auto state = loadStateFromFile(xmlString);
    // pluginGraph->deleteAndReplaceAudioBlocks(state);
}

void MainMenuInteractionHandler::onSave(std::string savePath)
{
    auto pluginState = XmlUtils::generatePluginState(mainGraphEditor->nodes);
    StorageManager::saveXML(savePath, pluginState->toString().toStdString());
}

void MainMenuInteractionHandler::onLoad(std::string filepath)
{
    mainGraphEditor->clear();

    auto file = File(filepath);
    auto xmlString = file.loadFileAsString().toStdString();
    auto state = mainGraphEditor->loadStateFromFile(xmlString);
    mainGraphEditor->pluginGraph->deleteAndReplaceAudioBlocks(state);
}