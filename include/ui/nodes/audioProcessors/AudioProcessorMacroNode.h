#pragma once
#include <dsp/AudioProcessorNode.h>
#include <dsp/Gain.h>
#include <ui/nodes/audioProcessors/AudioProcessorNodeUI.h>
#include <ui/parameters/NumberParameter.h>
#include <ui/windows/AudioProcessorMacroWindow.h>
#include <utils/Constants.h>

class GraphEditor;
class PluginGraph;

class AudioProcessorMacroNode : public AudioProcessorNodeUI
{
public:
    AudioProcessorMacroNode(PluginGraph* pluginGraph, String name=MACRO_DEFAULT_NAME);
    ~AudioProcessorMacroNode();

    void setUIParameters() override {};
    virtual void setAudioParametersFromXml(XmlElement* parametersXml) {};
    virtual XmlElement* getAudioParametersAsXml() {};

    void mouseDoubleClick(const MouseEvent& e) override;
    void connectInput(NodeUI* sourceNode) override;
    void connectFeedbackInput(NodeUI* sourceNode) override;
    void connectOutput(NodeUI* destinationNode) override;
    void disconnectInput(NodeUI* sourceNode) override;
    void disconnectOutput(NodeUI* destinationNode) override;
    std::string getIdAsString() override;
    

private:
    void closeWindow();
    std::unique_ptr<AudioProcessorMacroWindow> window;
    std::shared_ptr<GraphEditor> graphEditor;
};