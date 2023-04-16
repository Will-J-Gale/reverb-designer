/*
  ==============================================================================

    BiquadFilterNode.cpp
    Created: 20 Sep 2020 9:41:52pm
    Author:  Will

  ==============================================================================
*/
#include <ui/nodes/audioProcessors/BiquadFilterNode.h>
#include <utils/Constants.h>
#include <dsp/Parameters.h>
#include <utils/XmlUtils.h>

BiquadFilterNode::BiquadFilterNode(AudioProcessorNodeType type) : AudioProcessorNodeUI(type)
{
    setBounds(0, 0, FILTER_WIDTH, FILTER_HEIGHT);

    filterType.setSize(FILTER_WIDTH * 0.9, filterType.getHeight());
    filterType.setParameterName("Filter Type");
    frequency.setParameterName("Frequency");

    filterType.setTopLeftPosition(PARAMETER_X_OFFSET, PARAMETER_Y_OFFSET);
    frequency.setTopLeftPosition(PARAMETER_X_OFFSET, PARAMETER_Y_OFFSET + PARAMETER_SPACING);

    filterType.addItem("LPF", (int)FilterType::LPF1);
    filterType.addItem("HPF", (int)FilterType::HPF1);
    filterType.addItem("LSF", (int)FilterType::LSF);
    filterType.addItem("HSF", (int)FilterType::HSF);

    frequency.setValue(DEFAULT_FREQUENCY);
    
    frequency.addListener(this);
    filterType.addListener(this);

    addAndMakeVisible(filterType);
    addAndMakeVisible(frequency);
    
    addInputConnector();
    addOutputConnector();

    nameLabel.setBounds(0, 0, FILTER_WIDTH, TEXT_HEIGHT);
    updateNameAndReCenter(FILTER_NAME);
}

BiquadFilterNode::~BiquadFilterNode()
{
}

void BiquadFilterNode::textEditorTextChanged(TextEditor& textEditor)
{
    auto proc = getAudioProcessorAs<AudioFilter>();
    auto parameters = proc->getParameters();

    if (&textEditor == frequency.getComponent())
    {
        double value = textEditor.getText().getFloatValue();
        parameters.fc = value;
    }

    proc->setParameters(parameters);
}

void BiquadFilterNode::comboBoxChanged(ComboBox* comboBox)
{
    auto proc = dynamic_cast<AudioFilter*>(getAudioProcessor());
    auto parameters = proc->getParameters();

    if (comboBox == filterType.getComponent())
    {
        int choice = filterType.getSelectedItemId();

        parameters.filterType = (FilterType)choice;
    }

    proc->setParameters(parameters);
}

void BiquadFilterNode::setUIParameters()
{
    auto params = getAudioProcessorAs<AudioFilter>()->getParameters();
    filterType.setSelectedItem((int)params.filterType);
    frequency.setValue(params.fc);
}

void BiquadFilterNode::setAudioParametersFromXml(XmlElement* parametersXml)
{
    auto processor = getAudioProcessorAs<AudioFilter>();
    auto params = processor->getParameters();

    params.fc = parametersXml->getChildByName(CUTOFF_TAG)->getAllSubText().getFloatValue();
    params.filterType = (FilterType)(parametersXml->getChildByName(FILTER_TYPE_TAG)->getAllSubText().getIntValue());
    params.gain_dB = parametersXml->getChildByName(GAIN_TAG)->getAllSubText().getFloatValue();
    params.Q = parametersXml->getChildByName(Q_TAG)->getAllSubText().getFloatValue();

    processor->setParameters(params);
    setUIParameters();
}

XmlElement* BiquadFilterNode::getAudioParametersAsXml()
{
    auto params = getAudioProcessorAs<AudioFilter>()->getParameters();
    auto parametersXml = XmlUtils::createElement(PARAMETERTS_TAG);

    parametersXml->addChildElement(XmlUtils::createElement(CUTOFF_TAG, std::to_string(params.fc)));
    parametersXml->addChildElement(XmlUtils::createElement(FILTER_TYPE_TAG, std::to_string((int)params.filterType)));
    parametersXml->addChildElement(XmlUtils::createElement(GAIN_TAG, std::to_string(params.gain_dB)));
    parametersXml->addChildElement(XmlUtils::createElement(Q_TAG, std::to_string(params.Q)));

    return parametersXml;
}
