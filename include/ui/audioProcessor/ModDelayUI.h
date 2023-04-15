/*
  ==============================================================================

    ModDelayUI.h
    Created: 4 Oct 2020 6:08:27pm
    Author:  Will

  ==============================================================================
*/

#pragma once

#include <ui/audioProcessor/AudioProcessorUI.h>
#include <ui/parameters/ComboBoxParameter.h>
#include <ui/parameters/SliderParameter.h>

class ModDelayUI : public AudioProcessorUI,
                   public Slider::Listener,
                   public ComboBox::Listener
{
public:
    ModDelayUI(DspObjectType type);
    ~ModDelayUI();

    virtual void sliderValueChanged(Slider* slider) override;
    virtual void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;

    virtual void setAudioParametersFromXml(XmlElement* parametersXml) override;
    virtual XmlElement* getAudioParametersAsXml() override;

private:
    virtual void setUIParameters() override;

    ComboBoxParameter algorithm;
    SliderParameter feedback;
    SliderParameter lfoRate;
    SliderParameter lfoDepth;
};