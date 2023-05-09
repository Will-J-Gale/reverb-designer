#include <ui/nodes/audioProcessors/IO.h>
#include <utils/Constants.h>

IO::IO(String name, AudioProcessorNodePtr node, NodeInstance instance, AudioProcessorMacroNode* parent) : AudioProcessorNodeUI(name, instance, node)
{
    setBounds(0, 0, IO_WIDTH, IO_HEIGHT);
    updateNameAndReCenter(name);
    this->parent = parent;
}

std::string IO::getParentId()
{
    return parent->getIdAsString();
}