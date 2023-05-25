/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <utils/Constants.h>

//==============================================================================
ReverbDesignerAudioProcessorEditor::ReverbDesignerAudioProcessorEditor (ReverbDesignerAudioProcessor& p)
    : AudioProcessorEditor (&p), _processor (p)
{
   
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(PLUGIN_WIDTH, PLUGIN_HEIGHT);
    setResizable(true, true);

    _mainGraphEditor.setPluginGraph(_processor.getPluginGraph());
    _mainGraphEditor.setBounds(getBounds());
    addAndMakeVisible(_mainGraphEditor);
}

ReverbDesignerAudioProcessorEditor::~ReverbDesignerAudioProcessorEditor()
{
}

//==============================================================================
void ReverbDesignerAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void ReverbDesignerAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
