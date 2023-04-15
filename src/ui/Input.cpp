/*
  ==============================================================================

    Input.cpp
    Created: 18 Sep 2020 8:19:33pm
    Author:  Will

  ==============================================================================
*/

#include <ui/Input.h>
#include <utils/Constants.h>
#include <utils/XmlUtils.h>

Input::Input(DspObjectType type) : AudioProcessorUI(type)
{
    setBounds(0, 0, IO_WIDTH, IO_HEIGHT);
    nameLabel.setJustificationType(Justification::centred);
    nameLabel.setBounds(getBounds());
    updateNameAndReCenter("Input");

    addOutputNode();

    contextMenu.clear();
}

void Input::setAudioParametersFromXml(XmlElement* parametersXml)
{
    channel = parametersXml->getChildByName(IO_CHANNEL_TAG)->getAllSubText().getIntValue();
}

XmlElement* Input::getAudioParametersAsXml()
{
    auto parametersXml = XmlUtils::createElement(PARAMETERTS_TAG);

    parametersXml->addChildElement(XmlUtils::createElement(IO_CHANNEL_TAG, std::to_string(channel)));

    return parametersXml;
}

void Input::setChannel(int channel)
{
    this->channel = channel;
}
