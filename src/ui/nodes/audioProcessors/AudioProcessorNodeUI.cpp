#include <dsp/FeedbackDelay.h>
#include <ui/nodes/audioProcessors/AudioProcessorNodeUI.h>
#include <ui/GraphEditor.h>
#include <ui/parameters/NumberParameter.h>
#include <ui/parameters/BoolParameter.h>
#include <ui/parameters/SliderParameter.h>
#include <ui/parameters/ComboBoxParameter.h>
#include <utils/XmlUtils.h>

AudioProcessorNodeUI::AudioProcessorNodeUI(String name, NodeInstance nodeInstance, AudioProcessorNodePtr node) 
    : NodeUI(name, NodeClass::AudioProcessor, nodeInstance)
{
    initialize();
    this->processorNode = node;

    //@TODO Refactor this 
    auto parameters = node->getParameters();
    auto yPosition = PARAMETER_Y_OFFSET;
    for(ParameterPtr parameter : parameters->getAllParameters())
    {
        if(parameter->getType() == ParameterType::Boolean)
        {
            auto boolParameter = static_cast<BooleanParameter*>(parameter.get());
            auto parameterUI = std::make_shared<BoolParameter>();
            parameterUI->setParameterName(boolParameter->getName());
            parameterUI->setToggleState(boolParameter->getValue());
            parameterUI->setTopLeftPosition(PARAMETER_X_OFFSET, yPosition);
            parameterUI->addListener(this);
            addAndMakeVisible(parameterUI.get());

            this->parametersUI.add(parameterUI);
            componentIdToParameterMap.insert({parameterUI->getId(), parameter});
            parameterNameToUI.insert({parameter->getName(), parameterUI.get()});
        }
        else if(parameter->getType() == ParameterType::Double)
        {
            auto doubleParameter = static_cast<DoubleParameter*>(parameter.get());
            auto parameterUI = std::make_shared<SliderParameter>();
            parameterUI->setParameterName(doubleParameter->getName());
            parameterUI->setMinAndMax(doubleParameter->getMin(), doubleParameter->getMax());
            parameterUI->setValue(doubleParameter->getValue());
            parameterUI->setTopLeftPosition(PARAMETER_X_OFFSET, yPosition);
            parameterUI->addListener(this);
            addAndMakeVisible(parameterUI.get());

            this->parametersUI.add(parameterUI);
            componentIdToParameterMap.insert({parameterUI->getId(), parameter});
            parameterNameToUI.insert({parameter->getName(), parameterUI.get()});
        }
        else if(parameter->getType() == ParameterType::Option)
        {
            auto optionParameter = static_cast<OptionParameter*>(parameter.get());
            auto parameterUI = std::make_shared<ComboBoxParameter>();
            for(OptionItem item : optionParameter->getItems())
            {
                parameterUI->addItem(item.name, item.value);
            }

            parameterUI->setParameterName(optionParameter->getName());
            parameterUI->setSelectedItem(optionParameter->getValue());
            parameterUI->setTopLeftPosition(PARAMETER_X_OFFSET, yPosition);
            parameterUI->addListener(this);
            addAndMakeVisible(parameterUI.get());

            this->parametersUI.add(parameterUI);
            componentIdToParameterMap.insert({parameterUI->getId(), parameter});
            parameterNameToUI.insert({parameter->getName(), parameterUI.get()});
        }

        yPosition += PARAMETER_SPACING;
    }

    auto height = NODE_MIN_HEIGHT + (this->parametersUI.size() * NODE_SCALE_PER_PARAMETER);
    setBounds(0, 0, NODE_DEFAULT_WIDTH, height);

    nameLabel.setBounds(0, 0, NODE_DEFAULT_WIDTH, TEXT_HEIGHT);
    updateNameAndReCenter(this->name);
}

AudioProcessorNodeUI::~AudioProcessorNodeUI()
{
}

AudioProcessorNodePtr AudioProcessorNodeUI::getProcessorNode()
{
	return processorNode;
}

AudioParametersPtr AudioProcessorNodeUI::getAudioParameters()
{
	return processorNode->getParameters();
}
void AudioProcessorNodeUI::setAudioParameters(AudioParametersPtr parameters)
{
    processorNode->getProcessor()->setParameters(parameters);
}

void AudioProcessorNodeUI::updateParametersUI()
{
    auto parameters = getAudioParameters();
    for(ParameterPtr& parameter : parameters->getAllParameters())
    {
        std::string parameterName = parameter->getName();
        ParameterType parameterType = parameter->getType();

        if(parameterType == ParameterType::Boolean)
        {
            BoolParameter* toggle = static_cast<BoolParameter*>(parameterNameToUI.at(parameterName));
            toggle->setToggleState(parameters->getParameterValueByName<bool>(parameterName));
        }
        else if(parameterType == ParameterType::Double)
        {
            SliderParameter* slider = static_cast<SliderParameter*>(parameterNameToUI.at(parameterName));
            slider->setValue(parameters->getParameterValueByName<double>(parameterName));
        }
        else if(parameterType == ParameterType::Option)
        {
            ComboBoxParameter* combo = static_cast<ComboBoxParameter*>(parameterNameToUI.at(parameterName));
            combo->setSelectedItem(parameters->getParameterValueByName<int>(parameterName));
        }
    }
}

void AudioProcessorNodeUI::setProcessorNode(AudioProcessorNodePtr processorNode)
{
    // jassert(processorNode->getNodeInstance() == nodeInstance);

    this->processorNode = processorNode;
}

IAudioProcessor* AudioProcessorNodeUI::getAudioProcessor()
{
    return processorNode->getProcessor();
}

std::string AudioProcessorNodeUI::getIdAsString()
{
    return processorNode->getIdAsString();
}

void AudioProcessorNodeUI::connectInput(NodeUI* sourceNode)
{
    if (!inputConnections.contains(sourceNode))
    {
        inputConnections.add(sourceNode);
        auto audioNode = dynamic_cast<AudioProcessorNodeUI*>(sourceNode);
        this->processorNode->connectInput(audioNode->getProcessorNode().get());
    }
}

void AudioProcessorNodeUI::connectFeedbackInput(NodeUI* sourceNode)
{
    if (!feedbackConnections.contains(sourceNode))
    {
        feedbackConnections.add(sourceNode);
        auto audioNode = dynamic_cast<AudioProcessorNodeUI*>(sourceNode);
        this->processorNode->connectFeedbackInput(audioNode->getProcessorNode().get());
    }
}

void AudioProcessorNodeUI::connectOutput(NodeUI* destinationNode)
{
    if (!outputConnections.contains(destinationNode))
    {
        outputConnections.add(destinationNode);

        auto audioNode = dynamic_cast<AudioProcessorNodeUI*>(destinationNode);
        this->processorNode->connectOutput(audioNode->getProcessorNode().get());
    } 
}

void AudioProcessorNodeUI::disconnectInput(NodeUI* sourceNode)
{
    removeFromArray(inputConnections, sourceNode);
    auto audioNode = dynamic_cast<AudioProcessorNodeUI*>(sourceNode);
    this->processorNode->disconnectInput(audioNode->getProcessorNode().get());
}

void AudioProcessorNodeUI::disconnectOutput(NodeUI* destinationNode)
{
    removeFromArray(outputConnections, destinationNode);
    auto audioNode = dynamic_cast<AudioProcessorNodeUI*>(destinationNode);
    this->processorNode->disconnectOutput(audioNode->getProcessorNode().get());
}

void AudioProcessorNodeUI::buttonClicked (Button* button)
{
    BaseParameter* parameterUI  = static_cast<BaseParameter*>(button->getParentComponent());
    BooleanParameter* parameter = static_cast<BooleanParameter*>(componentIdToParameterMap.at(parameterUI->getId()).get());
    parameter->setValue(button->getToggleState());
}

void AudioProcessorNodeUI::sliderValueChanged (Slider* slider)
{
    BaseParameter* parameterUI  = static_cast<BaseParameter*>(slider->getParentComponent());
    DoubleParameter* parameter = static_cast<DoubleParameter*>(componentIdToParameterMap.at(parameterUI->getId()).get());
    parameter->setValue(slider->getValue());
}

void AudioProcessorNodeUI::comboBoxChanged (ComboBox* comboBox)
{
    BaseParameter* parameterUI  = static_cast<BaseParameter*>(comboBox->getParentComponent());
    OptionParameter* parameter = static_cast<OptionParameter*>(componentIdToParameterMap.at(parameterUI->getId()).get());
    parameter->setValue(comboBox->getSelectedId());
}

XmlElement* AudioProcessorNodeUI::toXml()
{
    XmlElement* xml = XmlUtils::generateNodeXml(this);
    XmlElement* audioParameters = XmlUtils::audioParametersToXML(getAudioParameters());
    xml->addChildElement(audioParameters);

    return xml;
}

void AudioProcessorNodeUI::fromXml(XmlElement* nodeXml)
{
    std::string name = nodeXml->getChildByName(NAME_TAG)->getAllSubText().toStdString();
    updateNameAndReCenter(name);

    auto x = nodeXml->getChildByName(X_TAG)->getAllSubText().getIntValue();
    auto y = nodeXml->getChildByName(Y_TAG)->getAllSubText().getIntValue();
    auto isReversed = nodeXml->getChildByName(IS_REVERSED_TAG)->getAllSubText().getIntValue();
    auto id = nodeXml->getChildByName(ID_TAG)->getAllSubText().toStdString();

    setTopLeftPosition(x, y);

    if(isReversed)
        reverse();

    XmlElement* audioParametesXml = nodeXml->getChildByName(AUDIO_PARAMETERTS_TAG);
    XmlElement* parameter = audioParametesXml->getChildByName(PARAMETER_TAG);
    auto parameters = getAudioParameters();

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