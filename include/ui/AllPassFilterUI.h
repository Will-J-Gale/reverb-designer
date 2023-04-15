/*
  ==============================================================================

    AllPassFilterUI.h
    Created: 25 Sep 2020 8:21:25pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include <ui/AudioProcessorUI.h>
#include <dsp/DelayAPF.h>
#include <ui/SliderParameter.h>
#include <ui/AudioProcessorUI.h>
#include <ui/BoolParameter.h>


class GraphEditor;

class AllPassFilterUI : public AudioProcessorUI,
                        public ToggleButton::Listener,
                        public Slider::Listener
{
public:
    AllPassFilterUI(DspObjectType type);
    ~AllPassFilterUI();

    virtual void setAudioParametersFromXml(XmlElement* parametersXml) override;
    virtual XmlElement* getAudioParametersAsXml() override;

private:
    SliderParameter delayTime; 
    SliderParameter apfG; 
    BoolParameter enableLPF;
    SliderParameter lpfG;
    BoolParameter enableLFO;
    SliderParameter lfoRate;
    SliderParameter lfoDepth;
    SliderParameter maxModulation;

    virtual void buttonClicked(Button* button) override;
    virtual void sliderValueChanged(Slider* slider) override;
    virtual void setUIParameters() override;

    
};