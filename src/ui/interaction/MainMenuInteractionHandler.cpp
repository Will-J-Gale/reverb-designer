#include <ui/interaction/MainMenuInteractionHandler.h>
#include <ui/MainGraphEditor.h>
#include <utils/XmlUtils.h>
#include <utils/StorageManager.h>
#include <dsp/PluginGraph.h>

MainMenuInteractionHandler::MainMenuInteractionHandler(MainGraphEditor* mainGraphEditor)
{
    _mainGraphEditor = mainGraphEditor;
}

void MainMenuInteractionHandler::onNewProject()
{
    auto io = Array<NodeUIPtr>();
    io.addArray(_mainGraphEditor->_inputs);
    io.addArray(_mainGraphEditor->_outputs);

    auto nodesToDelete = _mainGraphEditor->_nodes;
    nodesToDelete.removeValuesIn(io);
    _mainGraphEditor->_nodes.removeValuesNotIn(io);

    for (auto node : nodesToDelete)
    {
        if (!io.contains(node))
        {
            _mainGraphEditor->_nodeInteractionHandler.deleteNode(node.get());
        }
    }

    _mainGraphEditor->repaint();
}

void MainMenuInteractionHandler::onPresetSelected(Presets::Type presetId)
{
    _mainGraphEditor->clear();

    auto xmlString = Presets::getPreset(presetId);
    auto xml = parseXML(xmlString);
    onNewProject();
    _mainGraphEditor->fromXml(xml.get());
    xml->deleteAllChildElements();
}

void MainMenuInteractionHandler::onSave(std::string savePath)
{
    auto pluginState = _mainGraphEditor->toXml();
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
    _mainGraphEditor->fromXml(xml.get());
    xml->deleteAllChildElements();
}

void MainMenuInteractionHandler::onMacroSelected(File macroFile)
{
    auto macroXmlString = macroFile.loadFileAsString().toStdString();
    auto xml = parseXML(macroXmlString);
    auto macroNode = _mainGraphEditor->_nodeInteractionHandler.createMacroNodeFromXml(xml.get());
    _mainGraphEditor->_nodeInteractionHandler.initializeNode(macroNode);

    xml->deleteAllChildElements();
}