/*
  ==============================================================================

    Output.cpp
    Created: 18 Sep 2020 8:19:39pm
    Author:  Will

  ==============================================================================
*/

#include <ui/audioProcessor/Output.h>
#include <utils/Constants.h>
#include <utils/XmlUtils.h>

Output::Output(DspObjectType type) : AudioProcessorUI(type)
{
    setBounds(0, 0, IO_WIDTH, IO_HEIGHT);
    nameLabel.setJustificationType(Justification::centred);
    nameLabel.setBounds(getBounds());
    updateNameAndReCenter("Output");

    addInputNode();

    contextMenu.clear();
}

void Output::setChannel(int channel)
{
    this->channel = channel;
}

void Output::setAudioParametersFromXml(XmlElement* parametersXml)
{
    channel = parametersXml->getChildByName(IO_CHANNEL_TAG)->getAllSubText().getIntValue();
}

XmlElement* Output::getAudioParametersAsXml()
{
    auto parametersXml = XmlUtils::createElement(PARAMETERTS_TAG);

    parametersXml->addChildElement(XmlUtils::createElement(IO_CHANNEL_TAG, std::to_string(channel)));

    return parametersXml;
}