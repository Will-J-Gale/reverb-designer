#include <ui/GraphEditor.h>

class PluginGraph;

class MainGraphEditor : public GraphEditor
{
public:
    MainGraphEditor();
    ~MainGraphEditor();
    void setPluginGraph(PluginGraph* pluginGraph) override;
    
    // XmlElementPtr generatePluginState();
    void loadFromExistingState(XmlElement* state);
    std::shared_ptr<AudioProcessorState> loadStateFromFile(std::string xmlString);

private:
    void createIOProcessors();
    void createAllConnections(std::map<std::string, NodeUIPtr> processorUIMap, std::map<std::string, XmlElement*> xmlMap);
    void resized() override;

    MainMenu mainMenu;
};