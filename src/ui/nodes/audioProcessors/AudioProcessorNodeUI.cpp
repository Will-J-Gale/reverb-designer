#include <dsp/FeedbackDelay.h>
#include <ui/nodes/audioProcessors/AudioProcessorNodeUI.h>
#include <ui/GraphEditor.h>
#include <ui/parameters/NumberParameterUI.h>
#include <ui/parameters/BoolParameterUI.h>
#include <ui/parameters/SliderParameterUI.h>
#include <ui/parameters/ComboBoxParameterUI.h>
#include <utils/XmlUtils.h>

AudioProcessorNodeUI::AudioProcessorNodeUI(String name, NodeInstance nodeInstance, AudioProcessorNodePtr node) 
    : NodeUI(name, NodeClass::AudioProcessor, nodeInstance)
{
    initialize();
    _processorNode = node;

    //@TODO Refactor this 
    auto parameters = node->getParameters();
    auto yPosition = PARAMETER_Y_OFFSET;
    auto maxParameterWidth = 0;

    for(ParameterPtr parameter : parameters->getAllParameters())
    {
        std::shared_ptr<BaseParameterUI> parameterUI = nullptr;

        if(parameter->getType() == ParameterType::Boolean)
        {
            auto boolParameter = static_cast<BooleanParameter*>(parameter.get());
            auto boolParameterUI = std::make_shared<BoolParameterUI>();
            parameterUI = boolParameterUI;
            boolParameterUI->setParameterName(boolParameter->getName());
            boolParameterUI->setToggleState(boolParameter->getValue());
            boolParameterUI->setTopLeftPosition(PARAMETER_X_OFFSET, yPosition);
            boolParameterUI->addListener(this);
            
        }
        else if(parameter->getType() == ParameterType::Double)
        {
            auto doubleParameter = static_cast<DoubleParameter*>(parameter.get());
            auto sliderParameterUI = std::make_shared<SliderParameterUI>();
            parameterUI = sliderParameterUI;
            sliderParameterUI->setParameterName(doubleParameter->getName());
            sliderParameterUI->setMinAndMax(doubleParameter->getMin(), doubleParameter->getMax());
            sliderParameterUI->setValue(doubleParameter->getValue());
            sliderParameterUI->setTopLeftPosition(PARAMETER_X_OFFSET, yPosition);
            sliderParameterUI->addListener(this);
        }
        else if(parameter->getType() == ParameterType::Option)
        {
            auto optionParameter = static_cast<OptionParameter*>(parameter.get());
            auto comboBoxParameterUI = std::make_shared<ComboBoxParameterUI>();
            parameterUI = comboBoxParameterUI;

            for(OptionItem item : optionParameter->getItems())
            {
                comboBoxParameterUI->addItem(item.name, item.value);
            }

            comboBoxParameterUI->setParameterName(optionParameter->getName());
            comboBoxParameterUI->setSelectedItem(optionParameter->getValue());
            comboBoxParameterUI->setTopLeftPosition(PARAMETER_X_OFFSET, yPosition);
            comboBoxParameterUI->addListener(this);
        }

        addAndMakeVisible(parameterUI.get());
        _parametersUI.add(parameterUI);
        _componentIdToParameterMap.insert({parameterUI->getId(), parameter});
        _parameterNameToUI.insert({parameter->getName(), parameterUI.get()});

        if(parameterUI->getVisibleWidth() > maxParameterWidth)
            maxParameterWidth = parameterUI->getVisibleWidth();

        yPosition += parameterUI->getHeight();
    }

    auto height = NODE_MIN_HEIGHT + yPosition;
    setBounds(0, 0, maxParameterWidth + 20, height);

    _nameLabel.setBounds(0, 0, NODE_DEFAULT_WIDTH, TEXT_HEIGHT);
    updateNameAndReCenter(_name);
}

AudioProcessorNodeUI::~AudioProcessorNodeUI()
{
}

AudioProcessorNodePtr AudioProcessorNodeUI::getProcessorNode()
{
	return _processorNode;
}

AudioParametersPtr AudioProcessorNodeUI::getAudioParameters()
{
	return _processorNode->getParameters();
}
void AudioProcessorNodeUI::setAudioParameters(AudioParametersPtr parameters)
{
    _processorNode->getProcessor()->setParameters(parameters);
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
            BoolParameterUI* toggle = static_cast<BoolParameterUI*>(_parameterNameToUI.at(parameterName));
            toggle->setToggleState(parameters->getParameterValueByName<bool>(parameterName));
        }
        else if(parameterType == ParameterType::Double)
        {
            SliderParameterUI* slider = static_cast<SliderParameterUI*>(_parameterNameToUI.at(parameterName));
            slider->setValue(parameters->getParameterValueByName<double>(parameterName));
        }
        else if(parameterType == ParameterType::Option)
        {
            ComboBoxParameterUI* combo = static_cast<ComboBoxParameterUI*>(_parameterNameToUI.at(parameterName));
            combo->setSelectedItem(parameters->getParameterValueByName<int>(parameterName));
        }
    }
}

void AudioProcessorNodeUI::setProcessorNode(AudioProcessorNodePtr processorNode)
{
    // jassert(processorNode->getNodeInstance() == nodeInstance);

    _processorNode = processorNode;
}

IAudioProcessor* AudioProcessorNodeUI::getAudioProcessor()
{
    return _processorNode->getProcessor();
}

std::string AudioProcessorNodeUI::getIdAsString()
{
    return _processorNode->getIdAsString();
}

void AudioProcessorNodeUI::connectInput(NodeUI* sourceNode)
{
    if (!_inputConnections.contains(sourceNode))
    {
        _inputConnections.add(sourceNode);
        auto audioNode = dynamic_cast<AudioProcessorNodeUI*>(sourceNode);
        _processorNode->connectInput(audioNode->getProcessorNode().get());
    }
}

void AudioProcessorNodeUI::connectFeedbackInput(NodeUI* sourceNode)
{
    if (!_feedbackConnections.contains(sourceNode))
    {
        _feedbackConnections.add(sourceNode);
        auto audioNode = dynamic_cast<AudioProcessorNodeUI*>(sourceNode);
        _processorNode->connectFeedbackInput(audioNode->getProcessorNode().get());
    }
}

void AudioProcessorNodeUI::connectOutput(NodeUI* destinationNode)
{
    if (!_outputConnections.contains(destinationNode))
    {
        _outputConnections.add(destinationNode);

        auto audioNode = dynamic_cast<AudioProcessorNodeUI*>(destinationNode);
        _processorNode->connectOutput(audioNode->getProcessorNode().get());
    } 
}

void AudioProcessorNodeUI::disconnectInput(NodeUI* sourceNode)
{
    removeFromArray(_inputConnections, sourceNode);
    auto audioNode = dynamic_cast<AudioProcessorNodeUI*>(sourceNode);
    _processorNode->disconnectInput(audioNode->getProcessorNode().get());
}

void AudioProcessorNodeUI::disconnectOutput(NodeUI* destinationNode)
{
    removeFromArray(_outputConnections, destinationNode);
    auto audioNode = dynamic_cast<AudioProcessorNodeUI*>(destinationNode);
    _processorNode->disconnectOutput(audioNode->getProcessorNode().get());
}

void AudioProcessorNodeUI::clearConnections()
{
    _inputConnections.clear();
    _outputConnections.clear();
}


void AudioProcessorNodeUI::buttonClicked (Button* button)
{
    BaseParameterUI* parameterUI  = static_cast<BaseParameterUI*>(button->getParentComponent());
    BooleanParameter* parameter = static_cast<BooleanParameter*>(_componentIdToParameterMap.at(parameterUI->getId()).get());
    parameter->setValue(button->getToggleState());
}

void AudioProcessorNodeUI::sliderValueChanged (Slider* slider)
{
    BaseParameterUI* parameterUI  = static_cast<BaseParameterUI*>(slider->getParentComponent());
    DoubleParameter* parameter = static_cast<DoubleParameter*>(_componentIdToParameterMap.at(parameterUI->getId()).get());
    parameter->setValue(slider->getValue());
}

void AudioProcessorNodeUI::comboBoxChanged (ComboBox* comboBox)
{
    BaseParameterUI* parameterUI  = static_cast<BaseParameterUI*>(comboBox->getParentComponent());
    OptionParameter* parameter = static_cast<OptionParameter*>(_componentIdToParameterMap.at(parameterUI->getId()).get());
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