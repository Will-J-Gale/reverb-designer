#pragma once
#include <JuceHeader.h>

#define XmlElementPtr std::shared_ptr<XmlElement>

class XmlUtils
{
public:
    static XmlElement* createElement(String tagName, String value)
    {
        auto xml = new XmlElement(tagName);
        xml->addTextElement(value);
        return xml;
    }

    static XmlElement* createElement(String tagName)
    {
        return new XmlElement(tagName);
    }
};