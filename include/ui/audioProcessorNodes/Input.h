/*
  ==============================================================================

    Input.h
    Created: 18 Sep 2020 7:58:16pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include <ui/audioProcessorNodes/AudioProcessorNodeUI.h>

class GraphEditor;

class Input : public AudioProcessorNodeUI
{
public:
    Input(DspObjectType type);

    virtual void setAudioParametersFromXml(XmlElement* parametersXml);
    virtual XmlElement* getAudioParametersAsXml();

    void setChannel(int channel);

private:
    int channel = 0;
};