/*
  ==============================================================================

    SimpleDelayUI.h
    Created: 21 Sep 2020 8:53:30pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include "AudioProcessorUI.h"
#include "NumberParameter.h"
#include "IAudioProcessor.h"
#include "SimpleDelay.h"

class GraphEditor;

class SimpleDelayUI : public AudioProcessorUI,
                      public TextEditor::Listener
{
public:
    SimpleDelayUI(DspObjectType type);
    ~SimpleDelayUI();

    void textEditorTextChanged(TextEditor& textEditor) override;

    virtual void setAudioParametersFromXml(XmlElement* parametersXml) override;
    virtual XmlElement* getAudioParametersAsXml() override;

private:
    virtual void setUIParameters() override;
    NumberParameter delayTime;
};