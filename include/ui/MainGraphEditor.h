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
    void fromXml(XmlElement* xml, IdToAudioProcessorMap* idToProcessorMap=nullptr) override;

private:
    friend class MainMenuInteractionHandler;

    void createIOProcessors();
    void resized() override;

    MainMenu mainMenu;
    MainMenuInteractionHandler mainMenuInteractionHandler{this};
};