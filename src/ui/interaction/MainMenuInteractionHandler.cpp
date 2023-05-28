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
    _mainGraphEditor->clear();
    _mainGraphEditor->_pluginGraph->addAction(PluginGraphActionType::ClearAllNodes);
}

void MainMenuInteractionHandler::loadXMLCallback(std::string xmlString)
{
    const MessageManagerLock mmlock;
    _mainGraphEditor->clear();

    auto xml = parseXML(xmlString);
    _mainGraphEditor->fromXml(xml.get());
    _mainGraphEditor->_pluginGraph->addAction(PluginGraphActionType::CalculateProcessPath);
    xml->deleteAllChildElements();
}

void MainMenuInteractionHandler::onPresetSelected(Presets::Type presetId)
{
    auto xmlString = Presets::getPreset(presetId);
    
    //The callback method guarentees all nodes will be deleted when the callback is called
    auto callback = std::bind(&MainMenuInteractionHandler::loadXMLCallback, this, xmlString);
    _mainGraphEditor->_pluginGraph->addAction(PluginGraphActionType::ClearAllNodes, callback);
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

    auto callback = std::bind(&MainMenuInteractionHandler::loadXMLCallback, this, xmlString);
    _mainGraphEditor->_pluginGraph->addAction(PluginGraphActionType::ClearAllNodes, callback);
}

void MainMenuInteractionHandler::onMacroSelected(File macroFile)
{
    auto macroXmlString = macroFile.loadFileAsString().toStdString();
    auto xml = parseXML(macroXmlString);
    auto macroNode = _mainGraphEditor->_nodeInteractionHandler.createMacroNodeFromXml(xml.get());
    _mainGraphEditor->_nodeInteractionHandler.initializeNode(macroNode);

    xml->deleteAllChildElements();
}