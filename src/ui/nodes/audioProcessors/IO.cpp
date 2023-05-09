#include <ui/nodes/audioProcessors/IO.h>
#include <utils/Constants.h>
#include <utils/XmlUtils.h>

IO::IO(String name, int channel, AudioProcessorNodePtr node, NodeInstance instance, AudioProcessorMacroNode* parent) : AudioProcessorNodeUI(name, instance, node)
{
    setBounds(0, 0, IO_WIDTH, IO_HEIGHT);
    updateNameAndReCenter(name);
    this->parent = parent;
    this->channel = channel;
}

std::string IO::getParentId()
{
    return parent->getIdAsString();
}

int IO::getChannel()
{
    return channel;
}

XmlElement* IO::toXml()
{
    XmlElement* xml = XmlUtils::generateNodeXml(this);
    XmlUtils::createAndAddElement(IO_CHANNEL_TAG, std::to_string(channel), xml);
    return xml;
}
void IO::fromXml(XmlElement* xml)
{
    channel = xml->getChildByName(IO_CHANNEL_TAG)->getAllSubText().getIntValue();
}