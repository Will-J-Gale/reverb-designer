#pragma once
#include <JuceHeader.h>
#include <parameters/AudioParameters.h>
#include <ui/nodes/audioProcessors/AudioProcessorNodeUI.h>
#include <ui/nodes/audioProcessors/AudioProcessorMacroNode.h>
#include <ui/nodes/audioProcessors/IO.h>

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

    inline XmlElement* generateNodeXml(NodeUI* node)
    {
        XmlElement* xml = new XmlElement(NODE_TAG);
        createAndAddElement(NAME_TAG, node->getNodeName(), xml);
        createAndAddElement(NODE_CLASS_TAG, std::to_string((int)node->getNodeClass()), xml);
        createAndAddElement(INSTANCE_TAG, std::to_string((int)node->getNodeInstance()), xml);
        createAndAddElement(X_TAG, std::to_string(node->getX()), xml);
        createAndAddElement(Y_TAG, std::to_string(node->getY()), xml);
        createAndAddElement(IS_REVERSED_TAG, std::to_string(node->isReversed()), xml);
        createAndAddElement(ID_TAG, node->getIdAsString(), xml);

        return xml;
    }

    inline XmlElement* generateAudioNodeXml(AudioProcessorNodeUI* node)
    {
        XmlElement* nodeXml = generateNodeXml(node);
        XmlElement* parametersXml = audioParametersToXML(node->getAudioParameters());

        nodeXml->addChildElement(parametersXml);

        return nodeXml;
    }

    inline XmlElement* generateMacroNodeXml(AudioProcessorMacroNode* macroNode)
    {
        XmlElement* macroXml = generateNodeXml(macroNode);
        XmlElement* internalNodesXml = macroXml->createNewChildElement("InternalNodes");
        Array<NodeUIPtr> internalNodes = macroNode->getInternalNodes();

        for(NodeUIPtr& nodeUI : internalNodes)
        {
            XmlElement* nodeXml = nullptr;

            if(nodeUI->getNodeClass() == NodeClass::AudioProcessor)
                nodeXml = generateAudioNodeXml(static_cast<AudioProcessorNodeUI*>(nodeUI.get()));

            internalNodesXml->addChildElement(nodeXml);
        }

        return macroXml;
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
}