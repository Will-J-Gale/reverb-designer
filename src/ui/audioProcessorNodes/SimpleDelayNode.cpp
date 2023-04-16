/*
  ==============================================================================

    SimpleDelayNode.cpp
    Created: 21 Sep 2020 8:53:30pm
    Author:  Will

  ==============================================================================
*/

#include <ui/audioProcessorNodes/SimpleDelayNode.h>
#include <utils/Constants.h>
#include <ui/GraphEditor.h>
#include <utils/XmlUtils.h>

SimpleDelayNode::SimpleDelayNode(AudioProcessorNodeType type) : AudioProcessorNodeUI(type)
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

SimpleDelayNode::~SimpleDelayNode()
{
}

void SimpleDelayNode::textEditorTextChanged(TextEditor& textEditor)
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

void SimpleDelayNode::setUIParameters()
{
    auto params = getAudioProcessorAs<SimpleDelay>()->getParameters();
    delayTime.setValue(params.delayTimeInMs);
}

void SimpleDelayNode::setAudioParametersFromXml(XmlElement* parametersXml)
{
    auto processor = getAudioProcessorAs<SimpleDelay>();
    auto params = processor->getParameters();

    params.delayTimeInMs = parametersXml->getChildByName(DELAY_TIME_TAG)->getAllSubText().getFloatValue();

    processor->setParameters(params);
    setUIParameters();
}

XmlElement* SimpleDelayNode::getAudioParametersAsXml()
{
    auto params = getAudioProcessorAs<SimpleDelay>()->getParameters();
    auto parametersXml = XmlUtils::createElement(PARAMETERTS_TAG);

    parametersXml->addChildElement(XmlUtils::createElement(DELAY_TIME_TAG, std::to_string(params.delayTimeInMs)));

    return parametersXml;
}
