/*
  ==============================================================================

    GainNode.cpp
    Created: 19 Sep 2020 1:34:26pm
    Author:  Will

  ==============================================================================
*/

#include <ui/nodes/audioProcessors/GainNode.h>
#include <utils/Constants.h>
#include <utils/XmlUtils.h>

GainNode::GainNode() : AudioProcessorNodeUI(GAIN_NAME, NodeInstance::Gain)
{
    setBounds(0, 0, GAIN_WIDTH, GAIN_HEIGHT);

    addInputConnector();
    addOutputConnector();
    addAndMakeVisible(gainParameter);

    gainParameter.setTopLeftPosition(PARAMETER_X_OFFSET, PARAMETER_Y_OFFSET);
    gainParameter.setWidth(GAIN_INPUT_WIDTH);
    gainParameter.addListener(this);

    nameLabel.setBounds(0, 0, GAIN_WIDTH, TEXT_HEIGHT);
    updateNameAndReCenter(GAIN_NAME);
}

GainNode::~GainNode()
{
}

void GainNode::textEditorTextChanged(TextEditor& textEditor)
{
    auto proc = getAudioProcessorAs<Gain>();

    if (&textEditor == gainParameter.getComponent())
    {
        double value = textEditor.getText().getFloatValue();
        proc->setGain(value);
    }
}

void GainNode::setUIParameters()
{
    auto gain = getAudioProcessorAs<Gain>()->getGain();
    gainParameter.setValue(gain);
}

void GainNode::setAudioParametersFromXml(XmlElement* parametersXml)
{
    auto processor = getAudioProcessorAs<Gain>();
    auto gain = parametersXml->getChildByName(GAIN_TAG)->getAllSubText().getFloatValue();

    processor->setGain(gain);
    setUIParameters();
}

XmlElement* GainNode::getAudioParametersAsXml()
{
    auto processor = getAudioProcessorAs<Gain>();
    auto parametersXml = XmlUtils::createElement(PARAMETERTS_TAG);

    parametersXml->addChildElement(XmlUtils::createElement(GAIN_TAG, std::to_string(processor->getGain())));

    return parametersXml;
}
