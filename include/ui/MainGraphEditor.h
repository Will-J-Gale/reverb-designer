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
    
    void loadFromExistingState(XmlElement* state);
    void fromXml(XmlElement* xml, IdToAudioProcessorMap* idToProcessorMap=nullptr) override;

private:
    friend class MainMenuInteractionHandler;

    void createIOProcessors();
    void resized() override;

    MainMenu _mainMenu;
    MainMenuInteractionHandler _mainMenuInteractionHandler{this};
};