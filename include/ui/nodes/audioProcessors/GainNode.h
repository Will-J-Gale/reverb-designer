/*
  ==============================================================================

    GainNode.h
    Created: 19 Sep 2020 1:34:26pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include <ui/nodes/audioProcessors/AudioProcessorNodeUI.h>
#include <dsp/Gain.h>
#include <ui/parameters/NumberParameter.h>
#include <dsp/AudioProcessorNode.h>

class GainNode : public AudioProcessorNodeUI,
               public TextEditor::Listener
{
public:
    GainNode(NodeType type);
    ~GainNode();

    void textEditorTextChanged(TextEditor& textEditor) override;

    virtual void setAudioParametersFromXml(XmlElement* parametersXml) override;
    virtual XmlElement* getAudioParametersAsXml() override;

private:
    virtual void setUIParameters() override;

    NumberParameter gainParameter;
};