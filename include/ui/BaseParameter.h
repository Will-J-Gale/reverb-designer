/*
  ==============================================================================

    BaseParameter.h
    Created: 10 Sep 2020 5:03:09pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class BaseParameter : public Component
{
public:
    BaseParameter();
    void setParameterName(String newName);
    virtual Component* getComponent() = 0;

protected:
    Label name;
};