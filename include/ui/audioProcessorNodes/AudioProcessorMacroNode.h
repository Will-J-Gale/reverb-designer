#pragma once
#include <dsp/AudioProcessorNode.h>
#include <dsp/Gain.h>
#include <ui/audioProcessorNodes/AudioProcessorNodeUI.h>
#include <ui/parameters/NumberParameter.h>
#include <ui/windows/AudioProcessorMacroWindow.h>

class AudioProcessorMacroNode : public AudioProcessorNodeUI
{
public:
    AudioProcessorMacroNode(AudioProcessorNodeType type);
    ~AudioProcessorMacroNode();

    void setUIParameters() override {};
    virtual void setAudioParametersFromXml(XmlElement* parametersXml) {};
    virtual XmlElement* getAudioParametersAsXml() {};

    void mouseDoubleClick(const MouseEvent& e) override;

private:
    void closeWindow();
    std::unique_ptr<AudioProcessorMacroWindow> window;
};