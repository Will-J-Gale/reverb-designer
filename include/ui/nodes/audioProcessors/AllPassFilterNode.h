/*
  ==============================================================================

    AllPassFilterNode.h
    Created: 25 Sep 2020 8:21:25pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include <dsp/DelayAPF.h>
#include <ui/parameters/SliderParameter.h>
#include <ui/nodes/audioProcessors/AudioProcessorNodeUI.h>
#include <ui/parameters/BoolParameter.h>


class GraphEditor;

class AllPassFilterNode : public AudioProcessorNodeUI,
                        public ToggleButton::Listener,
                        public Slider::Listener
{
public:
    AllPassFilterNode();
    ~AllPassFilterNode();

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