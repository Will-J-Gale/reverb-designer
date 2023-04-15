/*
  ==============================================================================

    GainUI.h
    Created: 19 Sep 2020 1:34:26pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include <ui/AudioProcessorUI.h>
#include <dsp/Gain.h>
#include <ui/NumberParameter.h>
#include <dsp/AudioProcessingBlock.h>

class GainUI : public AudioProcessorUI,
               public TextEditor::Listener
{
public:
    GainUI(DspObjectType type);
    ~GainUI();

    void textEditorTextChanged(TextEditor& textEditor) override;

    virtual void setAudioParametersFromXml(XmlElement* parametersXml) override;
    virtual XmlElement* getAudioParametersAsXml() override;

private:
    virtual void setUIParameters() override;

    NumberParameter gainParameter;
};