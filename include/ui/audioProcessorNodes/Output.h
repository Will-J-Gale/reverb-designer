/*
  ==============================================================================

    Output.h
    Created: 18 Sep 2020 7:58:20pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include <ui/audioProcessorNodes/AudioProcessorNode.h>

//Forward declaration
class GraphEditor;

class Output : public AudioProcessorNode
{
public:
    Output(DspObjectType type);

    virtual void setAudioParametersFromXml(XmlElement* parametersXml);
    virtual XmlElement* getAudioParametersAsXml();

    void setChannel(int channel);

private:
    int channel = 0;
};