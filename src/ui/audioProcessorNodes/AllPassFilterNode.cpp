/*
  ==============================================================================

    AllPassFilterNode.cpp
    Created: 25 Sep 2020 8:21:25pm
    Author:  Will

  ==============================================================================
*/

#include <ui/audioProcessorNodes/AllPassFilterNode.h>
#include <utils/Constants.h>
#include <ui/GraphEditor.h>
#include <utils/XmlUtils.h>

AllPassFilterNode::AllPassFilterNode(AudioProcessorNodeType type) : AudioProcessorNodeUI(type)
{
    setBounds(0, 0, APF_WIDTH, APF_HEIGHT);
    updateNameAndReCenter(APF_NAME);

    delayTime.setTopLeftPosition(PARAMETER_X_OFFSET, PARAMETER_Y_OFFSET);
    apfG.setTopLeftPosition(PARAMETER_X_OFFSET, PARAMETER_Y_OFFSET + SLIDER_SPACING);
    enableLPF.setTopLeftPosition(PARAMETER_X_OFFSET, PARAMETER_Y_OFFSET + (SLIDER_SPACING * 2));
    lpfG.setTopLeftPosition(PARAMETER_X_OFFSET, PARAMETER_Y_OFFSET + (SLIDER_SPACING * 3));
    enableLFO.setTopLeftPosition(PARAMETER_X_OFFSET, PARAMETER_Y_OFFSET + (SLIDER_SPACING * 4));
    lfoRate.setTopLeftPosition(PARAMETER_X_OFFSET, PARAMETER_Y_OFFSET + (SLIDER_SPACING * 5));
    lfoDepth.setTopLeftPosition(PARAMETER_X_OFFSET, PARAMETER_Y_OFFSET + (SLIDER_SPACING * 6));
    maxModulation.setTopLeftPosition(PARAMETER_X_OFFSET, PARAMETER_Y_OFFSET + (SLIDER_SPACING * 7));

    delayTime.setParameterName("Delay Time");
    delayTime.setTextSuffix("ms");
    apfG.setParameterName("APF G");
    lpfG.setParameterName("LPF G");
    enableLPF.setParameterName("Enable LPF");
    enableLFO.setParameterName("Enable LFO");
    lfoRate.setParameterName("LFO Rate");
    lfoDepth.setParameterName("LFO Depth");
    maxModulation.setParameterName("Max Modulation");

    delayTime.addListener(this);
    apfG.addListener(this);
    enableLPF.addListener(this);
    lpfG.addListener(this);
    enableLFO.addListener(this);
    lfoRate.addListener(this);
    lfoDepth.addListener(this);
    maxModulation.addListener(this);

    delayTime.setMinAndMax(APF_MIN_DELAY, APF_MAX_DELAY);
    maxModulation.setMinAndMax(APF_MIN_MOD_FREQ, APF_MAX_MOD_FREQ);
    lfoRate.setMinAndMax(APF_LFO_RATE_MIN, APF_LFO_RATE_MAX);

    addAndMakeVisible(delayTime);
    addAndMakeVisible(apfG);
    addAndMakeVisible(enableLPF);
    addAndMakeVisible(lpfG);
    addAndMakeVisible(enableLFO);
    addAndMakeVisible(lfoRate);
    addAndMakeVisible(lfoDepth);
    addAndMakeVisible(maxModulation);

    addInputNode();
    addOutputNode();
}

AllPassFilterNode::~AllPassFilterNode()
{

}

void AllPassFilterNode::buttonClicked(Button* button)
{
    auto apf = getAudioProcessorAs<DelayAPF>();
    auto params = apf->getParameters();

    if (button == enableLPF.getComponent())
    {
        params.enableLPF = button->getToggleState();
    }
    else if (button == enableLFO.getComponent())
    {
        params.enableLFO = button->getToggleState();
    }

    apf->setParameters(params);
}

void AllPassFilterNode::sliderValueChanged(Slider* slider)
{
    auto apf = getAudioProcessorAs<DelayAPF>();
    auto params = apf->getParameters();

    if (slider == delayTime.getComponent())
    {
        params.delayTimeInMs = slider->getValue();
    }
    else if (slider == apfG.getComponent())
    {
        params.apfG = slider->getValue();
    }
    else if (slider == lpfG.getComponent())
    {
        params.lpfG = slider->getValue();
    }
    else if (slider == lfoRate.getComponent())
    {
        params.lfoRate = slider->getValue();
    }
    else if (slider == lfoDepth.getComponent())
    {
        params.lfoDepth = slider->getValue();
    }
    else if (slider == maxModulation.getComponent())
    {
        params.lfoMaxModulationInMs = slider->getValue();
    }

    apf->setParameters(params);
}

void AllPassFilterNode::setUIParameters()
{
    auto params = getAudioProcessorAs<DelayAPF>()->getParameters();

    delayTime.setValue(params.delayTimeInMs);
    apfG.setValue(params.apfG);
    enableLPF.setToggleState(params.enableLPF);
    lpfG.setValue(params.lpfG);
    enableLFO.setToggleState(params.enableLFO);
    lfoRate.setValue(params.lfoRate);
    lfoDepth.setValue(params.lfoDepth);
    maxModulation.setValue(params.lfoMaxModulationInMs);
}

void AllPassFilterNode::setAudioParametersFromXml(XmlElement* parametersXml)
{
    auto processor = getAudioProcessorAs<DelayAPF>();
    auto params = processor->getParameters();

    params.delayTimeInMs = parametersXml->getChildByName(DELAY_TIME_TAG)->getAllSubText().getFloatValue();
    params.apfG = parametersXml->getChildByName(APF_G_TAG)->getAllSubText().getFloatValue();
    params.enableLFO = parametersXml->getChildByName(ENABLE_LFO_TAG)->getAllSubText().getIntValue();
    params.lfoDepth = parametersXml->getChildByName(LFO_DEPTH_TAG)->getAllSubText().getFloatValue();
    params.lfoRate = parametersXml->getChildByName(LFO_RATE_TAG)->getAllSubText().getFloatValue();
    params.lfoMaxModulationInMs = parametersXml->getChildByName(MAX_MODULATION_TAG)->getAllSubText().getFloatValue();
    params.enableLPF = parametersXml->getChildByName(ENABLE_LPF_TAG)->getAllSubText().getIntValue();
    params.lpfG = parametersXml->getChildByName(LPF_G_TAG)->getAllSubText().getFloatValue();

    processor->setParameters(params);
    setUIParameters();
}

XmlElement* AllPassFilterNode::getAudioParametersAsXml()
{
    auto params = getAudioProcessorAs<DelayAPF>()->getParameters();
    auto parameterXml = XmlUtils::createElement(PARAMETERTS_TAG);

    parameterXml->addChildElement(XmlUtils::createElement(APF_G_TAG, std::to_string(params.apfG)));
    parameterXml->addChildElement(XmlUtils::createElement(DELAY_TIME_TAG, std::to_string(params.delayTimeInMs)));
    parameterXml->addChildElement(XmlUtils::createElement(ENABLE_LFO_TAG, std::to_string(params.enableLFO)));
    parameterXml->addChildElement(XmlUtils::createElement(LFO_DEPTH_TAG, std::to_string(params.lfoDepth)));
    parameterXml->addChildElement(XmlUtils::createElement(LFO_RATE_TAG, std::to_string(params.lfoRate)));
    parameterXml->addChildElement(XmlUtils::createElement(MAX_MODULATION_TAG, std::to_string(params.lfoMaxModulationInMs)));
    parameterXml->addChildElement(XmlUtils::createElement(ENABLE_LPF_TAG, std::to_string(params.enableLPF)));
    parameterXml->addChildElement(XmlUtils::createElement(LPF_G_TAG, std::to_string(params.lpfG)));

    return parameterXml;
}
