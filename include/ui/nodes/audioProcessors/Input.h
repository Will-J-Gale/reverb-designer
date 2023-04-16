/*
  ==============================================================================

    Input.h
    Created: 18 Sep 2020 7:58:16pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include <ui/nodes/NodeUI.h>

class Input : public NodeUI
{
public:
    Input();

    virtual void setAudioParametersFromXml(XmlElement* parametersXml);
    virtual XmlElement* getAudioParametersAsXml();

    void setChannel(int channel);

private:
    int channel = 0;
};