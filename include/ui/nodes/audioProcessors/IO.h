#pragma once
#include <ui/nodes/audioProcessors/AudioProcessorNodeUI.h>
#include <ui/nodes/audioProcessors/AudioProcessorMacroNode.h>

class IO : public AudioProcessorNodeUI
{
public:
    IO(String name, int channel, AudioProcessorNodePtr node, NodeInstance instance, AudioProcessorMacroNode* parent=nullptr);
    std::string getParentId();
    void handleRightClick() override {};
    int getChannel();
    virtual XmlElement* toXml() override;
    virtual void fromXml(XmlElement* xml) override;

private:
    AudioProcessorMacroNode* parent = nullptr;
    int channel = NULL;

};