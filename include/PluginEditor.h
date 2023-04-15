/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include <ui/AudioProcessorUI.h>
#include <ui/GraphEditor.h>

//==============================================================================
/**
*/
class ReverbDesignerAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    ReverbDesignerAudioProcessorEditor (ReverbDesignerAudioProcessor&);
    ~ReverbDesignerAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ReverbDesignerAudioProcessor& processor;

    GraphEditor graphEditor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReverbDesignerAudioProcessorEditor)
};
