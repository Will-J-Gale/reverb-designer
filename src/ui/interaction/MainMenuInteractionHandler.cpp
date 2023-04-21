#include <ui/interaction/MainMenuInteractionHandler.h>
#include <ui/GraphEditor.h>

MainMenuInteractionHandler::MainMenuInteractionHandler(GraphEditor* graphEditor)
{
    this->graphEditor = graphEditor;
}

void MainMenuInteractionHandler::onNewProject()
{
    auto io = Array<NodeUIPtr>();
    io.addArray(graphEditor->inputs);
    io.addArray(graphEditor->outputs);

    auto nodesToDelete = graphEditor->nodes;
    nodesToDelete.removeValuesIn(io);
    graphEditor->nodes.removeValuesNotIn(io);

    for (auto node : nodesToDelete)
    {
        if (!io.contains(node))
        {
            graphEditor->nodeInteractionHandler.deleteProcessor(node.get());
        }
    }

    graphEditor->repaint();
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
    // auto state = generatePluginState();
    // StorageManager::saveXML(savePath, state->toString().toStdString());
}

void MainMenuInteractionHandler::onLoad(std::string filepath)
{
    graphEditor->clear();

    // auto file = File(filepath);
    // auto xmlString = file.loadFileAsString().toStdString();
    // auto callback = std::bind(&GraphEditor::loadStateFromFile, this, xmlString);
    
    // pluginGraph->deleteAndReplaceAudioBlocks(callback);
}