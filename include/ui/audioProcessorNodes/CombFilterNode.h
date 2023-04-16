/*
  ==============================================================================

    CombFilterNode.h
    Created: 26 Sep 2020 3:51:16pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include <ui/audioProcessorNodes/AudioProcessorNodeUI.h>
#include <ui/parameters/SliderParameter.h>
#include <ui/parameters/BoolParameter.h>
#include <dsp/CombFilter.h>

class GraphEditor;

class CombFilterNode : public AudioProcessorNodeUI,
                     public Slider::Listener,
                     public ToggleButton::Listener
{
public:
    CombFilterNode(AudioProcessorNodeType type);
    ~CombFilterNode();

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