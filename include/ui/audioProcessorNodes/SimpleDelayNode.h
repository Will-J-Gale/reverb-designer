/*
  ==============================================================================

    SimpleDelayNode.h
    Created: 21 Sep 2020 8:53:30pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include <ui/audioProcessorNodes/AudioProcessorNode.h>
#include <ui/parameters/NumberParameter.h>
#include <dsp/IAudioProcessor.h>
#include <dsp/SimpleDelay.h>

class GraphEditor;

class SimpleDelayNode : public AudioProcessorNode,
                      public TextEditor::Listener
{
public:
    SimpleDelayNode(DspObjectType type);
    ~SimpleDelayNode();

    void textEditorTextChanged(TextEditor& textEditor) override;

    virtual void setAudioParametersFromXml(XmlElement* parametersXml) override;
    virtual XmlElement* getAudioParametersAsXml() override;

private:
    virtual void setUIParameters() override;
    NumberParameter delayTime;
};