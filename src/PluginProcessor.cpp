/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ReverbDesignerAudioProcessor::ReverbDesignerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

ReverbDesignerAudioProcessor::~ReverbDesignerAudioProcessor()
{
}

//==============================================================================
const String ReverbDesignerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ReverbDesignerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ReverbDesignerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ReverbDesignerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ReverbDesignerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ReverbDesignerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ReverbDesignerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ReverbDesignerAudioProcessor::setCurrentProgram (int index)
{
}

const String ReverbDesignerAudioProcessor::getProgramName (int index)
{
    return {};
}

void ReverbDesignerAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================

void ReverbDesignerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ReverbDesignerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void ReverbDesignerAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    pluginGraph->reset(sampleRate);  
    pluginGraph->addInputs(getNumInputChannels());
    pluginGraph->addOutputs(getNumOutputChannels());
}
void ReverbDesignerAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    float* leftData = buffer.getWritePointer(0);
    float* rightData = buffer.getWritePointer(1);
    auto channels = buffer.getArrayOfWritePointers();

    size_t numChannels = buffer.getNumChannels();

    for (int i = 0; i < buffer.getNumSamples(); i++)
    {
        std::unique_ptr<std::vector<float>> frame = std::make_unique<std::vector<float>>();

        for (int channel = 0; channel < numChannels; channel++)
        {
            frame->push_back(channels[channel][i]);
        }

        pluginGraph->process(*frame, numChannels);
        leftData[i] = pluginGraph->getOutputSampleFrom(0);
        rightData[i] = pluginGraph->getOutputSampleFrom(1);
    }
}

//==============================================================================
bool ReverbDesignerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* ReverbDesignerAudioProcessor::createEditor()
{
    return new ReverbDesignerAudioProcessorEditor (*this);
}

//==============================================================================
void ReverbDesignerAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ReverbDesignerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

PluginGraph* ReverbDesignerAudioProcessor::getPluginGraph()
{
    return pluginGraph.get();
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ReverbDesignerAudioProcessor();
}
