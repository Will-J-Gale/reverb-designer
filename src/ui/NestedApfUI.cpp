/*
  ==============================================================================

    NestedApfUI.cpp
    Created: 5 Oct 2020 6:53:26pm
    Author:  Will

  ==============================================================================
*/

#include <ui/NestedApfUI.h>
#include "Constants.h"
#include <dsp/NestedDelayAPF.h>
#include "XmlUtils.h"

NestedApfUI::NestedApfUI(DspObjectType type) : AudioProcessorUI(type)
{
    setBounds(0, 0, NESTED_APF_WIDTH, NESTED_APF_HEIGHT);
    updateNameAndReCenter(NESTED_APF_NAME);

    outerDelayTime.setTopLeftPosition(PARAMETER_X_OFFSET, PARAMETER_Y_OFFSET);
    outerApfG.setTopLeftPosition(PARAMETER_X_OFFSET, PARAMETER_Y_OFFSET + SLIDER_SPACING);
    innerDelayTime.setTopLeftPosition(PARAMETER_X_OFFSET, PARAMETER_Y_OFFSET + (SLIDER_SPACING * 2));
    innerApfG.setTopLeftPosition(PARAMETER_X_OFFSET, PARAMETER_Y_OFFSET + (SLIDER_SPACING * 3));
    enableLPF.setTopLeftPosition(PARAMETER_X_OFFSET, PARAMETER_Y_OFFSET + (SLIDER_SPACING * 4));
    lpfG.setTopLeftPosition(PARAMETER_X_OFFSET, PARAMETER_Y_OFFSET + (SLIDER_SPACING * 5));
    enableLFO.setTopLeftPosition(PARAMETER_X_OFFSET, PARAMETER_Y_OFFSET + (SLIDER_SPACING * 6));
    lfoRate.setTopLeftPosition(PARAMETER_X_OFFSET, PARAMETER_Y_OFFSET + (SLIDER_SPACING * 7));
    lfoDepth.setTopLeftPosition(PARAMETER_X_OFFSET, PARAMETER_Y_OFFSET + (SLIDER_SPACING * 8));
    maxModulation.setTopLeftPosition(PARAMETER_X_OFFSET, PARAMETER_Y_OFFSET + (SLIDER_SPACING * 9));

    outerDelayTime.setParameterName("Outer Delay Time");
    outerDelayTime.setTextSuffix("ms");
    outerApfG.setParameterName("Outer APF G");
    innerDelayTime.setParameterName("Inner Delay Time");
    innerDelayTime.setTextSuffix("ms");
    innerApfG.setParameterName("Inner APF G");
    lpfG.setParameterName("LPF G");
    enableLPF.setParameterName("Enable LPF");
    enableLFO.setParameterName("Enable LFO");
    lfoRate.setParameterName("LFO Rate");
    lfoDepth.setParameterName("LFO Depth");
    maxModulation.setParameterName("Max Modulation");

    outerDelayTime.addListener(this);
    outerApfG.addListener(this);
    innerDelayTime.addListener(this);
    innerApfG.addListener(this);
    enableLPF.addListener(this);
    lpfG.addListener(this);
    enableLFO.addListener(this);
    lfoRate.addListener(this);
    lfoDepth.addListener(this);
    maxModulation.addListener(this);

    outerDelayTime.setMinAndMax(APF_MIN_DELAY, APF_MAX_DELAY);
    innerDelayTime.setMinAndMax(APF_MIN_DELAY, APF_MAX_DELAY);
    maxModulation.setMinAndMax(APF_MIN_MOD_FREQ, APF_MAX_MOD_FREQ);
    lfoRate.setMinAndMax(APF_LFO_RATE_MIN, APF_LFO_RATE_MAX);

    addAndMakeVisible(outerDelayTime);
    addAndMakeVisible(outerApfG);
    addAndMakeVisible(innerDelayTime);
    addAndMakeVisible(innerApfG);
    addAndMakeVisible(enableLPF);
    addAndMakeVisible(lpfG);
    addAndMakeVisible(enableLFO);
    addAndMakeVisible(lfoRate);
    addAndMakeVisible(lfoDepth);
    addAndMakeVisible(maxModulation);

    addInputNode();
    addOutputNode();
}

void NestedApfUI::buttonClicked(Button* button)
{
    auto nApf = getAudioProcessorAs<NestedDelayAPF>();
    auto params = nApf->getParameters();

    if (button == enableLPF.getComponent())
    {
        params.enableLPF = button->getToggleState();
    }
    else if (button == enableLFO.getComponent())
    {
        params.enableLFO = button->getToggleState();
    }

    nApf->setParameters(params);
}

void NestedApfUI::sliderValueChanged(Slider* slider)
{
    auto nApf = getAudioProcessorAs<NestedDelayAPF>();
    auto params = nApf->getParameters();

    if (slider == outerDelayTime.getComponent())
    {
        params.outerDelayTimeInMs = slider->getValue();
    }
    else if (slider == outerApfG.getComponent())
    {
        params.outerApfG = slider->getValue();
    }
    else if (slider == innerDelayTime.getComponent())
    {
        params.innerDelayTimeMs = slider->getValue();
    }
    else if (slider == innerApfG.getComponent())
    {
        params.innerApfG = slider->getValue();
    }
    else if (slider == lpfG.getComponent())
    {
        params.lpfG = slider->getValue();
    }
    else if (slider == lfoRate.getComponent())
    {
        params.lfoRateHz = slider->getValue();
    }
    else if (slider == lfoDepth.getComponent())
    {
        params.lfoDepth = slider->getValue();
    }
    else if (slider == maxModulation.getComponent())
    {
        params.lfoMaxModulationInMs = slider->getValue();
    }

    nApf->setParameters(params);
}

void NestedApfUI::setUIParameters()
{
    auto params = getAudioProcessorAs<NestedDelayAPF>()->getParameters();

    outerDelayTime.setValue(params.outerDelayTimeInMs);
    outerApfG.setValue(params.outerApfG);

    innerDelayTime.setValue(params.innerDelayTimeMs);
    innerApfG.setValue(params.innerApfG);

    enableLPF.setToggleState(params.enableLPF);
    lpfG.setValue(params.lpfG);

    enableLFO.setToggleState(params.enableLFO);
    lfoRate.setValue(params.lfoRateHz);
    lfoDepth.setValue(params.lfoDepth);
    maxModulation.setValue(params.lfoMaxModulationInMs);
}

void NestedApfUI::setAudioParametersFromXml(XmlElement* parametersXml)
{
    auto processor = getAudioProcessorAs<NestedDelayAPF>();
    auto params = processor->getParameters();

    params.outerDelayTimeInMs = parametersXml->getChildByName(OUTER_DELAY_TIME_TAG)->getAllSubText().getFloatValue();
    params.outerApfG = parametersXml->getChildByName(OUTER_APF_G_TAG)->getAllSubText().getFloatValue();
    params.innerDelayTimeMs = parametersXml->getChildByName(INNER_DELAY_TIME_TAG)->getAllSubText().getFloatValue();
    params.innerApfG = parametersXml->getChildByName(INNER_APF_G_TAG)->getAllSubText().getFloatValue();
    params.enableLPF = parametersXml->getChildByName(ENABLE_LPF_TAG)->getAllSubText().getIntValue();
    params.lpfG = parametersXml->getChildByName(LPF_G_TAG)->getAllSubText().getFloatValue();
    params.enableLFO = parametersXml->getChildByName(ENABLE_LFO_TAG)->getAllSubText().getIntValue();
    params.lfoRateHz = parametersXml->getChildByName(LFO_RATE_TAG)->getAllSubText().getIntValue();
    params.lfoDepth = parametersXml->getChildByName(LFO_DEPTH_TAG)->getAllSubText().getIntValue();
    params.lfoMaxModulationInMs = parametersXml->getChildByName(MAX_MODULATION_TAG)->getAllSubText().getIntValue();

    processor->setParameters(params);
    setUIParameters();
}

XmlElement* NestedApfUI::getAudioParametersAsXml()
{
    auto params = getAudioProcessorAs<NestedDelayAPF>()->getParameters();
    auto parametersXml = XmlUtils::createElement(PARAMETERTS_TAG);

    parametersXml->addChildElement(XmlUtils::createElement(OUTER_APF_G_TAG, std::to_string(params.outerApfG)));
    parametersXml->addChildElement(XmlUtils::createElement(OUTER_DELAY_TIME_TAG, std::to_string(params.outerDelayTimeInMs)));
    parametersXml->addChildElement(XmlUtils::createElement(INNER_APF_G_TAG, std::to_string(params.innerApfG)));
    parametersXml->addChildElement(XmlUtils::createElement(INNER_DELAY_TIME_TAG, std::to_string(params.innerApfG)));
    parametersXml->addChildElement(XmlUtils::createElement(ENABLE_LFO_TAG, std::to_string(params.enableLFO)));
    parametersXml->addChildElement(XmlUtils::createElement(LFO_DEPTH_TAG, std::to_string(params.lfoDepth)));
    parametersXml->addChildElement(XmlUtils::createElement(LFO_RATE_TAG, std::to_string(params.lfoRateHz)));
    parametersXml->addChildElement(XmlUtils::createElement(MAX_MODULATION_TAG, std::to_string(params.lfoMaxModulationInMs)));
    parametersXml->addChildElement(XmlUtils::createElement(ENABLE_LPF_TAG, std::to_string(params.enableLPF)));
    parametersXml->addChildElement(XmlUtils::createElement(LPF_G_TAG, std::to_string(params.lpfG)));

    return parametersXml;
}
