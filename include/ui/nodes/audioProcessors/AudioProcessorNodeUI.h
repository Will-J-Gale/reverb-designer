/*
  ==============================================================================

    BaseAudioProcessor.h
    Created: 10 Sep 2020 6:52:28pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include <vector>
#include <JuceHeader.h>
#include <dsp/IAudioProcessor.h>
#include <dsp/AudioProcessorNode.h>
#include <ui/nodes/NodeConnectorUI.h>
#include <ui/menus/NodeUIContextMenu.h>
#include <ui/interaction/DragHandler.h>
#include <ui/nodes/NodeUI.h>
#include <utils/XmlUtils.h>
#include <utils/Constants.h>
#include <utils/ManagedArray.h>
#include <ui/parameters/BaseParameter.h>

//Forward declarations
class GraphEditor;

#define AudioProcessorNodeUIPtr std::shared_ptr<AudioProcessorNodeUI>

class AudioProcessorNodeUI :    public NodeUI,
                                public ToggleButton::Listener,
                                public Slider::Listener,
                                public ComboBox::Listener
{
public:
    AudioProcessorNodeUI(String name, NodeInstance nodeInstance, AudioProcessorNodePtr node);
    ~AudioProcessorNodeUI();
    
    AudioProcessorNodePtr getProcessorNode();
    void setProcessorNode(AudioProcessorNodePtr processorNode);
    IAudioProcessor* getAudioProcessor();
    
    virtual void connectInput(NodeUI* sourceNode) override;
    virtual void connectFeedbackInput(NodeUI* sourceNode) override;
    virtual void connectOutput(NodeUI* destinationNode) override;
    virtual void disconnectInput(NodeUI* sourceNode) override;
    virtual void disconnectOutput(NodeUI* destinationNode) override;
    void buttonClicked (Button* button) override;
    void sliderValueChanged (Slider* slider) override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    
    template<class T>
    T* getAudioProcessorAs();

    virtual std::string getIdAsString();

protected:
    AudioProcessorNodePtr processorNode = nullptr;
    Array<std::shared_ptr<BaseParameter>> parametersUI;
    std::map<std::string, ParameterPtr> componentIdToParameterMap;
};

template<class T>
inline T* AudioProcessorNodeUI::getAudioProcessorAs()
{
    return dynamic_cast<T*>(getAudioProcessor());
}