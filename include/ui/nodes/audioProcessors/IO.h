#pragma once
#include <ui/nodes/audioProcessors/AudioProcessorNodeUI.h>
#include <ui/nodes/audioProcessors/AudioProcessorMacroNode.h>

class IO : public AudioProcessorNodeUI
{
public:
    IO(String name, AudioProcessorNodePtr node, NodeInstance instance, AudioProcessorMacroNode* parent=nullptr);
    std::string getParentId();

private:
    AudioProcessorMacroNode* parent = nullptr;

};