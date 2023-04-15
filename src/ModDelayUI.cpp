/*
  ==============================================================================

    ModDelayUI.cpp
    Created: 4 Oct 2020 6:08:27pm
    Author:  Will

  ==============================================================================
*/

#include "ModDelayUI.h"
#include "Constants.h"
#include <dsp/ModulatedDelay.h>


ModDelayUI::ModDelayUI(DspObjectType type) : AudioProcessorUI(type)
{
    setBounds(0, 0, MOD_DELAY_WIDTH, MOD_DELAY_HEIGHT);
    updateNameAndReCenter(MOD_DELAY_NAME);

    algorithm.setTopLeftPosition(PARAMETER_X_OFFSET, PARAMETER_Y_OFFSET);
    feedback.setTopLeftPosition(PARAMETER_X_OFFSET, PARAMETER_Y_OFFSET + SLIDER_SPACING * 2);
    lfoRate.setTopLeftPosition(PARAMETER_X_OFFSET, PARAMETER_Y_OFFSET + (SLIDER_SPACING * 3));
    lfoDepth.setTopLeftPosition(PARAMETER_X_OFFSET, PARAMETER_Y_OFFSET + (SLIDER_SPACING * 4));

    algorithm.setParameterName("Algorithm");
    feedback.setParameterName("Feedback");
    lfoRate.setParameterName("LFO Rate");
    lfoDepth.setParameterName("LFO Depth");

    lfoRate.setMinAndMax(MOD_DELAY_LFO_RATE_MIN, MOD_DELAY_LFO_RATE_MAX);

    algorithm.addListener(this);
    feedback.addListener(this);
    lfoRate.addListener(this);
    lfoDepth.addListener(this);

    addAndMakeVisible(algorithm);
    addAndMakeVisible(feedback);
    addAndMakeVisible(lfoRate);
    addAndMakeVisible(lfoDepth);

    algorithm.addItem("Vibrato", (int)ModulatedDelayAlgorithm::kVibrato + 1);
    algorithm.addItem("Flanger", (int)ModulatedDelayAlgorithm::kFlanger + 1);
    algorithm.addItem("Chorus", (int)ModulatedDelayAlgorithm::kChorus + 1);

    addInputNode();
    addOutputNode();
}

ModDelayUI::~ModDelayUI()
{
}

void ModDelayUI::sliderValueChanged(Slider* slider)
{
    auto proc = getAudioProcessorAs<ModulatedDelay>();
    auto params = proc->getParameters();

    if (slider == feedback.getComponent())
    {
        params.feedback = slider->getValue();
    }
    else if (slider == lfoRate.getComponent())
    {
        params.lfoRate = slider->getValue();
    }
    else if (slider == lfoDepth.getComponent())
    {
        params.lfoDepth = slider->getValue();
    }

    proc->setParamters(params);

}

void ModDelayUI::comboBoxChanged(ComboBox* comboBox)
{
    auto proc = getAudioProcessorAs<ModulatedDelay>();
    auto params = proc->getParameters();

    params.algorithm = (ModulatedDelayAlgorithm)(comboBox->getSelectedId() - 1);
    proc->setParamters(params);
}

void ModDelayUI::setUIParameters()
{
    auto params = getAudioProcessorAs<ModulatedDelay>()->getParameters();

    algorithm.setSelectedItem((int)params.algorithm);
    feedback.setValue(params.feedback);
    lfoRate.setValue(params.lfoRate);
    lfoDepth.setValue(params.lfoDepth);
}

void ModDelayUI::setAudioParametersFromXml(XmlElement* parametersXml)
{
    auto processor = getAudioProcessorAs<ModulatedDelay>();
    auto params = processor->getParameters();

    params.algorithm = (ModulatedDelayAlgorithm)(parametersXml->getChildByName(MOD_ALGORITHM_TAG)->getAllSubText().getIntValue());
    params.feedback = parametersXml->getChildByName(FEEDBACK_TAG)->getAllSubText().getFloatValue();
    params.lfoRate = parametersXml->getChildByName(LFO_RATE_TAG)->getAllSubText().getFloatValue();
    params.lfoDepth = parametersXml->getChildByName(LFO_DEPTH_TAG)->getAllSubText().getFloatValue();

    processor->setParamters(params);
    setUIParameters();
}

XmlElement* ModDelayUI::getAudioParametersAsXml()
{
    auto params = getAudioProcessorAs<ModulatedDelay>()->getParameters();
    auto parametersXml = XmlUtils::createElement(PARAMETERTS_TAG);

    parametersXml->addChildElement(XmlUtils::createElement(MOD_ALGORITHM_TAG, std::to_string((int)params.algorithm)));
    parametersXml->addChildElement(XmlUtils::createElement(FEEDBACK_TAG, std::to_string(params.feedback)));
    parametersXml->addChildElement(XmlUtils::createElement(LFO_RATE_TAG, std::to_string(params.lfoRate)));
    parametersXml->addChildElement(XmlUtils::createElement(LFO_DEPTH_TAG, std::to_string(params.lfoDepth)));

    return parametersXml;
}
