#pragma once
#include <ui/GraphEditor.h>
#include <ui/interaction/MainMenuInteractionHandler.h>

class PluginGraph;

class MainGraphEditor : public GraphEditor
{
public:
    MainGraphEditor();
    ~MainGraphEditor();
    void setPluginGraph(PluginGraph* pluginGraph) override;
    
    // XmlElementPtr generatePluginState();
    void loadFromExistingState(XmlElement* state);

private:
    friend class MainMenuInteractionHandler;

    void createIOProcessors();
    // void createAllConnections(std::map<std::string, NodeUIPtr> processorUIMap, std::map<std::string, XmlElement*> xmlMap);
    void resized() override;

    MainMenu mainMenu;
    MainMenuInteractionHandler mainMenuInteractionHandler{this};
};