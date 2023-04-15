/*
  ==============================================================================

    CombFilterUI.cpp
    Created: 26 Sep 2020 3:51:16pm
    Author:  Will

  ==============================================================================
*/

#include <ui/audioProcessor/CombFilterUI.h>
#include <utils/Constants.h>
#include <utils/XmlUtils.h>

CombFilterUI::CombFilterUI(DspObjectType type) : AudioProcessorUI(type)
{
    setBounds(0, 0, COMB_WIDTH, COMB_HEIGHT);
    updateNameAndReCenter(COMB_NAME);

    delayTime.setTopLeftPosition(PARAMETER_X_OFFSET, PARAMETER_Y_OFFSET);
    rt60.setTopLeftPosition(PARAMETER_X_OFFSET, PARAMETER_Y_OFFSET + SLIDER_SPACING);
    lpfG.setTopLeftPosition(PARAMETER_X_OFFSET, PARAMETER_Y_OFFSET + (SLIDER_SPACING * 2));
    enableLPF.setTopLeftPosition(PARAMETER_X_OFFSET, PARAMETER_Y_OFFSET + (SLIDER_SPACING * 3));

    delayTime.setParameterName("Delay Time");
    rt60.setParameterName("RT60 Ms");
    lpfG.setParameterName("LPF G");
    enableLPF.setParameterName("Enable LPF");

    delayTime.setMinAndMax(0, 1000);
    rt60.setMinAndMax(0, 1000);

    delayTime.addListener(this);
    rt60.addListener(this);
    lpfG.addListener(this);
    enableLPF.addListener(this);

    addAndMakeVisible(delayTime);
    addAndMakeVisible(rt60);
    addAndMakeVisible(lpfG);
    addAndMakeVisible(enableLPF);

    addInputNode();
    addOutputNode();
}

CombFilterUI::~CombFilterUI()
{
}

void CombFilterUI::buttonClicked(Button* button)
{
    auto proc = getAudioProcessorAs<CombFilter>();
    auto parameters = proc->getParameters();

    if (button == enableLPF.getComponent())
    {
        parameters.enableLpf = button->getToggleState();
    }

    proc->setParameters(parameters);
}

void CombFilterUI::sliderValueChanged(Slider* slider)
{
    auto proc = getAudioProcessorAs<CombFilter>();
    auto parameters = proc->getParameters();

    if (slider == delayTime.getComponent())
    {
        parameters.delayTimeInMs = slider->getValue();
    }
    else if (slider == rt60.getComponent())
    {
        parameters.RT60TimeInMs = slider->getValue();
    }
    else if (slider == lpfG.getComponent())
    {
        parameters.lpfG = slider->getValue();
    }

    proc->setParameters(parameters);
}

void CombFilterUI::setUIParameters()
{
    auto params = getAudioProcessorAs<CombFilter>()->getParameters();

    delayTime.setValue(params.delayTimeInMs);
    rt60.setValue(params.RT60TimeInMs);
    lpfG.setValue(params.lpfG);
    enableLPF.setToggleState(params.enableLpf);
}

void CombFilterUI::setAudioParametersFromXml(XmlElement* parametersXml)
{
    auto processor = getAudioProcessorAs<CombFilter>();
    auto params = processor->getParameters();

    params.delayTimeInMs = parametersXml->getChildByName(DELAY_TIME_TAG)->getAllSubText().getFloatValue();
    params.enableLpf = parametersXml->getChildByName(ENABLE_LPF_TAG)->getAllSubText().getIntValue();
    params.lpfG = parametersXml->getChildByName(LPF_G_TAG)->getAllSubText().getFloatValue();
    params.RT60TimeInMs = parametersXml->getChildByName(REVERB_TIME_TAG)->getAllSubText().getFloatValue();

    processor->setParameters(params);
    setUIParameters();
}

XmlElement* CombFilterUI::getAudioParametersAsXml()
{
    auto params = getAudioProcessorAs<CombFilter>()->getParameters();
    auto parametersXml = XmlUtils::createElement(PARAMETERTS_TAG);

    parametersXml->addChildElement(XmlUtils::createElement(DELAY_TIME_TAG, std::to_string(params.delayTimeInMs)));
    parametersXml->addChildElement(XmlUtils::createElement(ENABLE_LPF_TAG, std::to_string(params.enableLpf)));
    parametersXml->addChildElement(XmlUtils::createElement(LPF_G_TAG, std::to_string(params.lpfG)));
    parametersXml->addChildElement(XmlUtils::createElement(REVERB_TIME_TAG, std::to_string(params.RT60TimeInMs)));

    return parametersXml;
}
