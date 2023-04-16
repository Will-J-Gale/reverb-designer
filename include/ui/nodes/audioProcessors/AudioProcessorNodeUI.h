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

//Forward declarations
class GraphEditor;

#define AudioProcessorNodeUIPtr std::shared_ptr<AudioProcessorNodeUI>

class AudioProcessorNodeUI : public NodeUI
{
public:
    AudioProcessorNodeUI(String name);
    AudioProcessorNodeUI(AudioProcessorNodeType type);
    ~AudioProcessorNodeUI();
    
    void addInputConnector();
    void addOutputConnector();
    Array<NodeConnectorUI*> getAllNodeConnectors();
    NodeConnectorUI* getInputNode();
    NodeConnectorUI* getOutputNode();
    
    void connectInput(NodeConnectorUI* connection);
    void connectFeedbackInput(NodeConnectorUI* connection);
    void connectOutput(NodeConnectorUI* connection);
    void disconnectInput(NodeConnectorUI* connection);
    void disconnectOutput(NodeConnectorUI* connection);
    Array<NodeConnectorUI*> getOutputConnections();
    Array<NodeConnectorUI*> getInputConnections();
    Array<NodeConnectorUI*> getFeedbackConnections();
    
    void addExistingInput(NodeConnectorUI* outputProcessor);
    void addExistingOutput(NodeConnectorUI* inputProcessor);

    AudioProcessorNodePtr getProcessorNode();
    void setProcessorNode(AudioProcessorNodePtr processorNode);

    IAudioProcessor* getAudioProcessor();
    AudioProcessorNodeType getType();
    
    bool isReversed();
    void reverse();

    template<class T>
    T* getAudioProcessorAs();

    virtual std::string getIdAsString();
    virtual void setUIParameters() {};
    virtual void setAudioParametersFromXml(XmlElement* parametersXml) = 0;
    virtual XmlElement* getAudioParametersAsXml() = 0;

protected:
    NodePtr input = nullptr;
    NodePtr output = nullptr;
    AudioProcessorNodePtr processorNode = nullptr;
    Array<NodeConnectorUI*> inputConnections;
    Array<NodeConnectorUI*> outputConnections;
    Array<NodeConnectorUI*> feedbackConnections;
    AudioProcessorNodeType type;
};

template<class T>
inline T* AudioProcessorNodeUI::getAudioProcessorAs()
{
    return dynamic_cast<T*>(getAudioProcessor());
}