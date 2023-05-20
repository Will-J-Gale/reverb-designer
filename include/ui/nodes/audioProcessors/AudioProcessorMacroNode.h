#pragma once
#include <dsp/AudioProcessorNode.h>
#include <dsp/Gain.h>
#include <ui/nodes/audioProcessors/AudioProcessorNodeUI.h>
#include <ui/parameters/NumberParameterUI.h>
#include <ui/windows/AudioProcessorMacroWindow.h>
#include <ui/menus/MacroContextMenu.h>
#include <utils/Constants.h>

class GraphEditor;
class PluginGraph;

#define AudioProcessorMacroNodePtr std::shared_ptr<AudioProcessorMacroNode>

class AudioProcessorMacroNode : public AudioProcessorNodeUI
{
public:
    AudioProcessorMacroNode(PluginGraph* pluginGraph, String name, AudioProcessorNodePtr node);
    ~AudioProcessorMacroNode();

    void mouseDoubleClick(const MouseEvent& e) override;
    void connectInput(NodeUI* sourceNode) override;
    void connectFeedbackInput(NodeUI* sourceNode) override;
    void connectOutput(NodeUI* destinationNode) override;
    void disconnectInput(NodeUI* sourceNode) override;
    void disconnectOutput(NodeUI* destinationNode) override;
    std::string getIdAsString() override;
    NodeUIPtr getInputNodeUI();
    NodeUIPtr getOutputNodeUI();
    Array<NodeUIPtr> getInternalNodes();
    Array<AudioProcessorNodePtr> getInternalAudioProcessorNodes();

    virtual XmlElement* toXml() override;
    virtual void fromXml(XmlElement* xml) override;

private:
    void closeWindow();
    std::unique_ptr<AudioProcessorMacroWindow> window;
    std::shared_ptr<GraphEditor> graphEditor;
};