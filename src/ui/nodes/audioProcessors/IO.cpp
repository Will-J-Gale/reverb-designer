#include <ui/nodes/audioProcessors/IO.h>
#include <utils/Constants.h>
#include <utils/XmlUtils.h>

IO::IO(String name, int channel, AudioProcessorNodePtr node, NodeInstance instance, AudioProcessorMacroNode* parent) : AudioProcessorNodeUI(name, instance, node)
{
    setBounds(0, 0, IO_WIDTH, IO_HEIGHT);
    updateNameAndReCenter(name);
    _parent = parent;
    _channel = channel;
}

std::string IO::getParentId()
{
    return _parent->getIdAsString();
}

int IO::getChannel()
{
    return _channel;
}

XmlElement* IO::toXml()
{
    XmlElement* xml = XmlUtils::generateNodeXml(this);
    XmlUtils::createAndAddElement(IO_CHANNEL_TAG, std::to_string(_channel), xml);
    return xml;
}
void IO::fromXml(XmlElement* xml)
{
    _channel = xml->getChildByName(IO_CHANNEL_TAG)->getAllSubText().getIntValue();
    auto x = xml->getChildByName(X_TAG)->getAllSubText().getIntValue();
    auto y = xml->getChildByName(Y_TAG)->getAllSubText().getIntValue();
    setTopLeftPosition(x, y);
}