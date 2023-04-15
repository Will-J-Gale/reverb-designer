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

class AudioProcessorNode;
class IAudioProcessor;

#define AudioProcessorNodePtr std::shared_ptr<AudioProcessorNode>
#define XmlElementPtr std::shared_ptr<XmlElement>

namespace XmlGenerator
{    
    /// Generates XmlElement for single AudioProcessorUI
    inline XmlElement* generateProcessorXml(AudioProcessorNodePtr processor)
    {
        XmlElement* xml = new XmlElement(PROCESSOR_STATE_TAG);
        XmlElement* xmlType = xml->createNewChildElement(TYPE_TAG);
        XmlElement* xmlX = xml->createNewChildElement(X_TAG);
        XmlElement* xmlY = xml->createNewChildElement(Y_TAG);
        XmlElement* xmlIsReversed = xml->createNewChildElement(IS_REVERSED_TAG);
        XmlElement* xmlId = xml->createNewChildElement(ID_TAG);
        
        xmlType->addTextElement(std::to_string((int)processor->getType()));
        xmlX->addTextElement(std::to_string(processor->getX()));
        xmlY->addTextElement(std::to_string(processor->getY()));
        xmlIsReversed->addTextElement(std::to_string(processor->isReversed()));
        xmlId->addTextElement(processor->getIdAsString());
        xml->addChildElement(processor->getAudioParametersAsXml());

        return xml;
    }
    
    /// Creates map between Uuid and XmlEmelent to help build processor from XML
    inline std::map<std::string, XmlElement*> generateXmlMap(XmlElement* pluginState)
    {
        auto xmlMap = std::map<std::string, XmlElement*>();

        XmlElement* inputs(pluginState->getChildByName(INPUTS_TAG));
        XmlElement* outputs(pluginState->getChildByName(OUTPUTS_TAG));
        XmlElement* processors(pluginState->getChildByName(PROCESSORS_TAG));

        XmlElement* inputXml(inputs->getChildByName(PROCESSOR_STATE_TAG));
        while (inputXml != nullptr)
        {
            auto id = inputXml->getChildByName(ID_TAG)->getAllSubText().toStdString();
            xmlMap[id] = inputXml;
            inputXml = inputXml->getNextElement();
        }

        XmlElement* outputXml(outputs->getChildByName(PROCESSOR_STATE_TAG));
        while (outputXml != nullptr)
        {
            auto id = outputXml->getChildByName(ID_TAG)->getAllSubText().toStdString();
            xmlMap[id] = outputXml;
            outputXml = outputXml->getNextElement();
        }

        XmlElement* processorXml(processors->getChildByName(PROCESSOR_STATE_TAG));
        while (processorXml != nullptr)
        {
            auto id = processorXml->getChildByName(ID_TAG)->getAllSubText().toStdString();
            xmlMap[id] = processorXml;
            processorXml = processorXml->getNextElement();
        }

        return xmlMap;
    }

    /// Generates XmlElement that holds state of whole plugin
    inline XmlElementPtr generatePluginState(Array<AudioProcessorNodePtr>& inputs, Array<AudioProcessorNodePtr>& outputs, Array<AudioProcessorNodePtr>& processors)
    {
        XmlElementPtr xml = std::make_shared<XmlElement>(PLUGIN_STATE_TAG);
        XmlElement* xmlInputs = xml->createNewChildElement(INPUTS_TAG);
        XmlElement* xmlOutputs = xml->createNewChildElement(OUTPUTS_TAG);
        XmlElement* xmlProcessors = xml->createNewChildElement(PROCESSORS_TAG);

        std::map<AudioProcessorNodePtr, XmlElement*> xmlMap;

        //Save states of processors and create state map
        for (auto input : inputs)
        {
            auto inputXml = generateProcessorXml(input);
            xmlInputs->addChildElement(inputXml);
            xmlMap[input] = inputXml;
        }

        for (auto output : outputs)
        {
            auto outputXml = generateProcessorXml(output);
            xmlOutputs->addChildElement(outputXml);
            xmlMap[output] = outputXml;
        }

        for (auto processor : processors)
        {
            auto processorXml = generateProcessorXml(processor);
            xmlProcessors->addChildElement(processorXml);
            xmlMap[processor] = processorXml;
        }

        //Create connections from state map
        for (auto pair : xmlMap)
        {
            auto processor = pair.first;
            auto processorXml = pair.second;

            auto inputConnections = processor->getInputConnections();
            auto outputConnections = processor->getOutputConnections();
            auto feedbackConnections = processor->getFeedbackConnections();

            if (!inputConnections.isEmpty())
            {
                auto inputConnectionsXml = processorXml->createNewChildElement(INPUT_CONNECTIONS_TAG);
                for (auto inputProcessor : inputConnections)
                {
                    //auto inputXml = xmlMap[inputProcessor];
                    auto idXml = new XmlElement(ID_TAG);
                    idXml->addTextElement(inputProcessor->getIdAsString());
                    inputConnectionsXml->addChildElement(idXml);
                }
            }
            
            if (!outputConnections.isEmpty())
            {
                auto outputConnectionsXml = processorXml->createNewChildElement(OUTPUT_CONNECTIONS_TAG);
                for (auto outputProcessor : outputConnections)
                {
                    //auto outputXml = xmlMap[outputProcessor];
                    auto idXml = new XmlElement(ID_TAG);
                    idXml->addTextElement(outputProcessor->getIdAsString());
                    outputConnectionsXml->addChildElement(idXml);
                }
            }

            if (!feedbackConnections.isEmpty())
            {
                auto feedbackConnectionsXml = processorXml->createNewChildElement(FEEDBACK_CONNECTIONS_TAG);
                for (auto feedbackProcessor : feedbackConnections)
                {
                    //auto outputXml = xmlMap[feedbackProcessor];
                    auto idXml = new XmlElement(ID_TAG);
                    idXml->addTextElement(feedbackProcessor->getIdAsString());
                    feedbackConnectionsXml->addChildElement(idXml);
                }
            }
        }

        return xml;
    }
};