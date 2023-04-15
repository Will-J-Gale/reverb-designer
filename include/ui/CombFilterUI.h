/*
  ==============================================================================

    CombFilterUI.h
    Created: 26 Sep 2020 3:51:16pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include <ui/AudioProcessorUI.h>
#include <ui/SliderParameter.h>
#include <ui/BoolParameter.h>
#include <dsp/CombFilter.h>

class GraphEditor;

class CombFilterUI : public AudioProcessorUI,
                     public Slider::Listener,
                     public ToggleButton::Listener
{
public:
    CombFilterUI(DspObjectType type);
    ~CombFilterUI();

    virtual void buttonClicked(Button*) override;
    virtual void sliderValueChanged(Slider* slider) override;

    virtual void setAudioParametersFromXml(XmlElement* xmlParameters) override;
    virtual XmlElement* getAudioParametersAsXml() override;
private:
    virtual void setUIParameters() override;

    SliderParameter delayTime;
    SliderParameter rt60;
    SliderParameter lpfG;
    BoolParameter enableLPF;
};