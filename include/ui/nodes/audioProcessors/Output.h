/*
  ==============================================================================

    Output.h
    Created: 18 Sep 2020 7:58:20pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include <ui/nodes/NodeUI.h>

class Output : public NodeUI
{
public:
    Output();

    virtual void setAudioParametersFromXml(XmlElement* parametersXml);
    virtual XmlElement* getAudioParametersAsXml();

    void setChannel(int channel);

private:
    int channel = 0;
};