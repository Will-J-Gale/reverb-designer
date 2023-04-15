/*
  ==============================================================================

    SimpleDelayUI.cpp
    Created: 21 Sep 2020 8:53:30pm
    Author:  Will

  ==============================================================================
*/

#include <ui/SimpleDelayUI.h>
#include "Constants.h"
#include <ui/GraphEditor.h>
#include "XmlUtils.h"

SimpleDelayUI::SimpleDelayUI(DspObjectType type) : AudioProcessorUI(type)
{
    setBounds(0, 0, DELAY_WIDTH, DELAY_HEIGHT);
    addInputNode();
    addOutputNode();

    delayTime.setBounds(PARAMETER_X_OFFSET, PARAMETER_Y_OFFSET, DELAY_TIME_WIDTH, DELAY_TIME_HEIGHT);
    delayTime.setParameterName(DELAY_TIME_TEXT);
    delayTime.setWidth(DELAY_INPUT_WIDTH);
    delayTime.addListener(this);
    addAndMakeVisible(delayTime);

    nameLabel.setBounds(0, 0, DELAY_WIDTH, TEXT_HEIGHT);
    updateNameAndReCenter(DELAY_NAME);
}

SimpleDelayUI::~SimpleDelayUI()
{
}

void SimpleDelayUI::textEditorTextChanged(TextEditor& textEditor)
{
    auto proc = getAudioProcessorAs<SimpleDelay>();
    auto parameters = proc->getParameters();

    if (&textEditor == delayTime.getComponent())
    {
        double value = textEditor.getText().getFloatValue();
        parameters.delayTimeInMs = value;
    }

    proc->setParameters(parameters);
}

void SimpleDelayUI::setUIParameters()
{
    auto params = getAudioProcessorAs<SimpleDelay>()->getParameters();
    delayTime.setValue(params.delayTimeInMs);
}

void SimpleDelayUI::setAudioParametersFromXml(XmlElement* parametersXml)
{
    auto processor = getAudioProcessorAs<SimpleDelay>();
    auto params = processor->getParameters();

    params.delayTimeInMs = parametersXml->getChildByName(DELAY_TIME_TAG)->getAllSubText().getFloatValue();

    processor->setParameters(params);
    setUIParameters();
}

XmlElement* SimpleDelayUI::getAudioParametersAsXml()
{
    auto params = getAudioProcessorAs<SimpleDelay>()->getParameters();
    auto parametersXml = XmlUtils::createElement(PARAMETERTS_TAG);

    parametersXml->addChildElement(XmlUtils::createElement(DELAY_TIME_TAG, std::to_string(params.delayTimeInMs)));

    return parametersXml;
}
