#pragma once
#include <JuceHeader.h>
#include <memory>
#include <parameters/AudioParameters.h>
#include <ui/nodes/audioProcessors/AudioProcessorNodeUI.h>

class NodeUI;
class IAudioProcessor;

#define NodeUIPtr std::shared_ptr<NodeUI>
#define XmlElementPtr std::shared_ptr<XmlElement>

namespace XmlUtils
{    
    inline XmlElement* createElement(String tagName, String value)
    {
        auto xml = new XmlElement(tagName);
        xml->addTextElement(value);
        return xml;
    }

    inline XmlElement* createElement(String tagName)
    {
        return new XmlElement(tagName);
    }

    inline XmlElement* createAndAddElement(String tagName, String value, XmlElement* parent)
    {
        XmlElement* xml = parent->createNewChildElement(tagName);
        xml->addTextElement(value);

        return xml;
    }

    inline XmlElement* audioParametersToXML(AudioParametersPtr audioParameters)
    {
        XmlElement* xml = new XmlElement(AUDIO_PARAMETERS_TAG);

        for(ParameterPtr& parameter : audioParameters->getAllParameters())
        {
            ParameterType parameterType = parameter->getType();
            XmlElement* parameterXml = xml->createNewChildElement(PARAMETER_TAG);
            createAndAddElement(NAME_TAG, parameter->getName(), parameterXml);
            createAndAddElement(TYPE_TAG, std::to_string((int)parameter->getType()), parameterXml);
            XmlElement* valueXml = parameterXml->createNewChildElement(VALUE_TAG);

            if(parameterType == ParameterType::Boolean)
            {
                bool value = audioParameters->getParameterValueByName<bool>(parameter->getName());
                valueXml->addTextElement(std::to_string(value));
            }
            else if(parameterType == ParameterType::Double)
            {
                double value = audioParameters->getParameterValueByName<double>(parameter->getName());
                valueXml->addTextElement(std::to_string(value));
            }
            else if(parameterType == ParameterType::Option)
            {
                int value = audioParameters->getParameterValueByName<int>(parameter->getName());
                valueXml->addTextElement(std::to_string(value));
            }
        }

        return xml;
    }

    // inline AudioParametersPtr xmlToAudioParameters(XmlElement* parametersXml)
    // {
    //     XmlElement* parameter = parametersXml->getChildByName(PARAMETER_TAG);
    //     std::vector<ParameterPtr> parameters; 

    //     while(parameter != nullptr)
    //     {
    //         std::string parameterName = parameter->getChildByName(NAME_TAG)->getAllSubText().toStdString();
    //         ParameterType parameterType = (ParameterType)parameter->getChildByName(TYPE_TAG)->getAllSubText().getIntValue();
    //         if(parameterType == ParameterType::Boolean)
    //         {
    //             parameters.push_back(
    //                 std::make_shared<BooleanParameter>(
    //                     parameterName,
    //                     (bool)parameter->getChildByName(VALUE_TAG)->getAllSubText().getIntValue()
    //                 )
    //             );
    //         }
    //         else if(parameterType == ParameterType::Double)
    //         {
    //             parameters.push_back(
    //                 std::make_shared<DoubleParameter>(
    //                     parameterName,
    //                     parameter->getChildByName(VALUE_TAG)->getAllSubText().getFloatValue(),
    //                     parameter->getChildByName(MIN_TAG)->getAllSubText().getFloatValue(),
    //                     parameter->getChildByName(MAX_TAG)->getAllSubText().getFloatValue()
    //                 )
    //             );
    //         }
    //         else if(parameterType == ParameterType::Option)
    //         {
                

    //             // auto options = std::vector<OptionItem>();
    //             // parameters.push_back(
    //             //     make_shared<OptionParameter>(parameterName, )
    //             // )
    //         }

    //         parameter = parameter->getNextElement();
    //     }

    //     return std::make_shared<AudioParameters>(parameters);
    // }

    inline void setAudioParametersFromXml(AudioParametersPtr parameters, XmlElement* parametersXml)
    {
        XmlElement* parameter = parametersXml->getChildByName(PARAMETER_TAG);

        while(parameter != nullptr)
        {
            std::string parameterName = parameter->getChildByName(NAME_TAG)->getAllSubText().toStdString();
            ParameterType parameterType = (ParameterType)parameter->getChildByName(TYPE_TAG)->getAllSubText().getIntValue();

            if(parameterType == ParameterType::Boolean)
            {
                bool value = (bool)parameter->getChildByName(VALUE_TAG)->getAllSubText().getIntValue();
                parameters->setParameterValueByName<bool>(parameterName, value);
            }
            else if(parameterType == ParameterType::Double)
            {
                double value = parameter->getChildByName(VALUE_TAG)->getAllSubText().getFloatValue();
                parameters->setParameterValueByName<double>(parameterName, value);
            }
            else if(parameterType == ParameterType::Option)
            {
                int value = parameter->getChildByName(VALUE_TAG)->getAllSubText().getIntValue();
                parameters->setParameterValueByName<int>(parameterName, value);
            }

            parameter = parameter->getNextElement();
        }
    }

    /// Generates XmlElement for single AudioProcessorUI
    inline XmlElement* generateNodeXml(NodeUI* node)
    {
        XmlElement* xml = new XmlElement(NODE_TAG);
        createAndAddElement(NODE_CLASS_TAG, std::to_string((int)node->getNodeClass()), xml);
        createAndAddElement(INSTANCE_TAG, std::to_string((int)node->getNodeInstance()), xml);
        createAndAddElement(X_TAG, std::to_string(node->getX()), xml);
        createAndAddElement(Y_TAG, std::to_string(node->getY()), xml);
        createAndAddElement(IS_REVERSED_TAG, std::to_string(node->isReversed()), xml);
        createAndAddElement(ID_TAG, node->getIdAsString(), xml);

        return xml;
    }

    /// Generates XmlElement for single AudioProcessorUI
    inline XmlElement* generateAudioNodeXml(AudioProcessorNodeUI* node)
    {
        XmlElement* nodeXml = generateNodeXml(node);
        XmlElement* parametersXml = audioParametersToXML(node->getAudioParameters());

        nodeXml->addChildElement(parametersXml);

        return nodeXml;
    }
    
    /// Creates map between Uuid and XmlEmelent to help build processor from XML
    inline std::map<std::string, XmlElement*> generateIdToNodeXmlElementMap(XmlElement* pluginState)
    {
        auto xmlMap = std::map<std::string, XmlElement*>();

        XmlElement* nodeXml(pluginState->getChildByName(NODE_TAG));
        while (nodeXml != nullptr)
        {
            auto id = nodeXml->getChildByName(ID_TAG)->getAllSubText().toStdString();
            xmlMap[id] = nodeXml;
            nodeXml = nodeXml->getNextElement();
        }

        return xmlMap;
    }

    /// Generates XmlElement that holds state of whole plugin
    inline XmlElementPtr generatePluginState(Array<NodeUIPtr>& nodes)
    {
        XmlElementPtr pluginStateXml = std::make_shared<XmlElement>(PLUGIN_STATE_TAG);
        std::map<NodeUIPtr, XmlElement*> xmlMap;

        for (auto node : nodes)
        {
            XmlElement* nodeXml = nullptr;

            if(node->getNodeClass() == NodeClass::AudioProcessor)
                nodeXml = generateAudioNodeXml(dynamic_cast<AudioProcessorNodeUI*>(node.get()));
            else
                nodeXml = generateNodeXml(node.get());

            pluginStateXml->addChildElement(nodeXml);
            xmlMap[node] = nodeXml;
        }

        //Create connections from state map
        for (auto pair : xmlMap)
        {
            auto node = pair.first;
            auto nodeXml = pair.second;

            auto inputConnections = node->getInputConnections();
            auto outputConnections = node->getOutputConnections();
            auto feedbackConnections = node->getFeedbackConnections();

            if (!inputConnections.isEmpty())
            {
                auto inputConnectionsXml = nodeXml->createNewChildElement(INPUT_CONNECTIONS_TAG);
                for (auto inputProcessor : inputConnections)
                {
                    auto idXml = new XmlElement(ID_TAG);
                    idXml->addTextElement(inputProcessor->getIdAsString());
                    inputConnectionsXml->addChildElement(idXml);
                }
            }
            
            if (!outputConnections.isEmpty())
            {
                auto outputConnectionsXml = nodeXml->createNewChildElement(OUTPUT_CONNECTIONS_TAG);
                for (auto outputProcessor : outputConnections)
                {
                    auto idXml = new XmlElement(ID_TAG);
                    idXml->addTextElement(outputProcessor->getIdAsString());
                    outputConnectionsXml->addChildElement(idXml);
                }
            }

            if (!feedbackConnections.isEmpty())
            {
                auto feedbackConnectionsXml = nodeXml->createNewChildElement(FEEDBACK_CONNECTIONS_TAG);
                for (auto feedbackProcessor : feedbackConnections)
                {
                    auto idXml = new XmlElement(ID_TAG);
                    idXml->addTextElement(feedbackProcessor->getIdAsString());
                    feedbackConnectionsXml->addChildElement(idXml);
                }
            }
        }

        return pluginStateXml;
    }
};