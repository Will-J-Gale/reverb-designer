/*
  ==============================================================================

    BiquadFilterNode.h
    Created: 20 Sep 2020 9:41:52pm
    Author:  Will

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <ui/audioProcessorNodes/AudioProcessorNodeUI.h>
#include <dsp/AudioFilter.h>
#include <ui/parameters/ComboBoxParameter.h>
#include <ui/parameters/NumberParameter.h>

class BiquadFilterNode : public AudioProcessorNodeUI,
                 public TextEditor::Listener,
                 public ComboBox::Listener
{
public:
    BiquadFilterNode(AudioProcessorNodeType type);
    ~BiquadFilterNode();

    void textEditorTextChanged(TextEditor& textEditor) override;
    virtual void comboBoxChanged(ComboBox* comboBox) override;

    virtual void setAudioParametersFromXml(XmlElement* parametersXml) override;
    virtual XmlElement* getAudioParametersAsXml() override;

private:
    virtual void setUIParameters() override;

    ComboBoxParameter filterType;
    NumberParameter frequency;
};



