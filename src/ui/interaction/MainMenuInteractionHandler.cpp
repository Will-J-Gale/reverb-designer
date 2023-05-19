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
            mainGraphEditor->nodeInteractionHandler.deleteNode(node.get());
        }
    }

    mainGraphEditor->repaint();
}

void MainMenuInteractionHandler::onPresetSelected(Presets::Type presetId)
{
    mainGraphEditor->clear();

    auto xmlString = Presets::getPreset(presetId);
    auto xml = parseXML(xmlString);
    onNewProject();
    mainGraphEditor->fromXml(xml.get());
    xml->deleteAllChildElements();
}

void MainMenuInteractionHandler::onSave(std::string savePath)
{
    auto pluginState = mainGraphEditor->toXml();
    StorageManager::saveXML(savePath, pluginState->toString().toStdString());
    pluginState->deleteAllChildElements();
    delete pluginState;
}

void MainMenuInteractionHandler::onLoad(std::string filepath)
{
    auto file = File(filepath);
    auto xmlString = file.loadFileAsString().toStdString();
    auto xml = parseXML(xmlString);
    onNewProject();
    mainGraphEditor->fromXml(xml.get());
    xml->deleteAllChildElements();
}