/*
  ==============================================================================

    PluginState.h
    Created: 10 Oct 2020 1:29:03pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <memory>

class AudioProcessorUI;
class IAudioProcessor;

#define AudioProcessorUIPtr std::shared_ptr<AudioProcessorUI>
#define XmlElementPtr std::shared_ptr<XmlElement>

class XmlGenerator
{
public:
    /// <summary>
    /// Generates XmlElement that holds state of whole plugin
    /// </summary>
    static XmlElementPtr generatePluginState(Array<AudioProcessorUIPtr>& inputs, Array<AudioProcessorUIPtr>& outputs, Array<AudioProcessorUIPtr>& processors);
    
    /// <summary>
    /// Generates XmlElement for single AudioProcessorUI
    /// </summary>
    static XmlElement* generateProcessorXml(AudioProcessorUIPtr processor);
    
    /// <summary>
    /// Creates map between Uuid and XmlEmelent to help build processor from XML
    /// </summary>
    static std::map<std::string, XmlElement*> generateXmlMap(XmlElement* pluginState);
};