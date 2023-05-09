#include <utils/Constants.h>
#include <ui/nodes/NodeUI.h>
#include <ui/nodes/audioProcessors/IO.h>
#include <ui/nodes/audioProcessors/AudioProcessorNodeUI.h>
#include <dsp/AudioProcessorNode.h>

namespace NodeFactory
{
    inline NodeUIPtr Generate(NodeInstance instance, AudioProcessorNodePtr node, AudioProcessorMacroNode* parent=nullptr)
    {
        String name = NodeInstanceToName.at(instance);

        if(instance == NodeInstance::Input || instance == NodeInstance::Output)
        {
            return std::make_shared<IO>(name, node, instance, parent);
        }
        else
        {
            return std::make_shared<AudioProcessorNodeUI>(name, instance, node);
        }
    }
}