/*
  ==============================================================================

    NestedApfUI.h
    Created: 5 Oct 2020 6:53:26pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include "AudioProcessorUI.h"
#include <JuceHeader.h>
#include "BoolParameter.h"
#include "SliderParameter.h"

class GraphEditor;

class NestedApfUI : public AudioProcessorUI,
                    public ToggleButton::Listener,
                    public Slider::Listener
{
public:
    NestedApfUI(DspObjectType type);

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