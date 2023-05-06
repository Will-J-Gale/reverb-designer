#include <ui/nodes/audioProcessors/AudioProcessorNodeUI.h>
#include <ui/GraphEditor.h>
#include <dsp/FeedbackDelay.h>
#include <ui/parameters/NumberParameter.h>
#include <ui/parameters/BoolParameter.h>
#include <ui/parameters/SliderParameter.h>
#include <ui/parameters/ComboBoxParameter.h>

AudioProcessorNodeUI::AudioProcessorNodeUI(String name, NodeInstance nodeInstance, AudioProcessorNodePtr node) 
    : NodeUI(name, NodeClass::AudioProcessor, nodeInstance)
{
    initialize();
    this->processorNode = processorNode;

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

    //@TODO Remove magic numbers
    int width = 200;
    auto height = NODE_MIN_HEIGHT + (this->parametersUI.size() * 50);
    setBounds(0, 0, width, height);

    nameLabel.setBounds(0, 0, width, TEXT_HEIGHT);
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