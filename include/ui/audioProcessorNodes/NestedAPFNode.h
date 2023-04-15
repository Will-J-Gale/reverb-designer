/*
  ==============================================================================

    NestedAPFNode.h
    Created: 5 Oct 2020 6:53:26pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include <ui/audioProcessorNodes/AudioProcessorNode.h>
#include <JuceHeader.h>
#include <ui/parameters/BoolParameter.h>
#include <ui/parameters/SliderParameter.h>

class GraphEditor;

class NestedAPFNode : public AudioProcessorNode,
                    public ToggleButton::Listener,
                    public Slider::Listener
{
public:
    NestedAPFNode(DspObjectType type);

    virtual void setAudioParametersFromXml(XmlElement* parametersXml) override;
    virtual XmlElement* getAudioParametersAsXml() override;

private:
    SliderParameter outerDelayTime;
    SliderParameter outerApfG;

    SliderParameter innerDelayTime;
    SliderParameter innerApfG;

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