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
#include <ui/audioProcessorNodes/AudioProcessorNodeConnectorUI.h>
#include <utils/Constants.h>
#include <utils/ManagedArray.h>
#include <dsp/IAudioProcessor.h>
#include <dsp/AudioProcessorNode.h>
#include <ui/menus/AudioProcessorUIContextMenu.h>
#include <utils/XmlUtils.h>
#include <ui/interaction/DragHandler.h>

//Forward declarations
class GraphEditor;

#define AudioProcessorNodeUIPtr std::shared_ptr<AudioProcessorNodeUI>

class AudioProcessorNodeUI : public Component,
                         public ManagedArray
{
public:
    class Listener
    {
    public:
        virtual void onProcessorClicked(AudioProcessorNodeUI* processor, const MouseEvent& e) = 0;
        virtual void onProcessorMoved(AudioProcessorNodeUI* processor, const MouseEvent& e) = 0;
        virtual void onProcessorReleased(AudioProcessorNodeUI* processor, const MouseEvent& e) = 0;
        virtual void onContextSelection(AudioProcessorNodeUI* processor, AudioProcessorConextMenuItems selection) = 0;
    };

    AudioProcessorNodeUI();
    AudioProcessorNodeUI(AudioProcessorNodeType type);
    ~AudioProcessorNodeUI();
    void paint(Graphics& g) override;
    void mouseDown(const MouseEvent& e) override;
    void mouseDrag(const MouseEvent& e) override;
    void mouseUp(const MouseEvent& e) override;
    void handleRightClick();
    void updateNameAndReCenter(String name);
    
    void addInputNode();
    void addOutputNode();
    Array<AudioProcessorNodeConnectorUI*> getAllNodes();
    AudioProcessorNodeConnectorUI* getInputNode();
    AudioProcessorNodeConnectorUI* getOutputNode();
    
    void connectInput(AudioProcessorNodeUI* connection);
    void connectFeedbackInput(AudioProcessorNodeUI* connection);
    void connectOutput(AudioProcessorNodeUI* connection);
    void disconnectInput(AudioProcessorNodeUI* connection);
    void disconnectOutput(AudioProcessorNodeUI* connection);
    Array<AudioProcessorNodeUI*> getOutputConnections();
    Array<AudioProcessorNodeUI*> getInputConnections();
    Array<AudioProcessorNodeUI*> getFeedbackConnections();
    
    virtual void setUIParameters() {};

    void addExistingInput(AudioProcessorNodeUI* outputProcessor);
    void addExistingOutput(AudioProcessorNodeUI* inputProcessor);
    
    void addListener(AudioProcessorNodeUI::Listener* listener);

    AudioProcessorNodePtr getProcessorNode();
    void setProcessorNode(AudioProcessorNodePtr processorNode);

    IAudioProcessor* getAudioProcessor();
    AudioProcessorNodeType getType();
    
    bool isReversed();
    void reverse();

    virtual std::string getIdAsString();

    template<class T>
    T* getAudioProcessorAs();

    virtual void setAudioParametersFromXml(XmlElement* parametersXml) = 0;
    virtual XmlElement* getAudioParametersAsXml() = 0;

protected:
    void initialize();
    Point<int> getCenterPosition();
    
    Uuid id;

    NodePtr input = nullptr;
    NodePtr output = nullptr;

    AudioProcessorNodePtr processorNode = nullptr;

    Array<AudioProcessorNodeUI*> inputConnections;
    Array<AudioProcessorNodeUI*> outputConnections;
    Array<AudioProcessorNodeUI*> feedbackConnections;

    Label nameLabel;
    String name;

    AudioProcessorNodeType type;

    bool reversed = false;

    AudioProcessorUIContextMenu contextMenu;

    Array<Listener*> listeners;

    DragHandler dragHandler = DragHandler(this);
};

template<class T>
inline T* AudioProcessorNodeUI::getAudioProcessorAs()
{
    return dynamic_cast<T*>(getAudioProcessor());
}