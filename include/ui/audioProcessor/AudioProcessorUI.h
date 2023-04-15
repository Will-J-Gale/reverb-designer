/*
  ==============================================================================

    BaseAudioProcessor.h
    Created: 10 Sep 2020 6:52:28pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <ui/audioProcessor/Node.h>
#include <vector>
#include <utils/Constants.h>
#include <utils/ManagedArray.h>
#include <dsp/IAudioProcessor.h>
#include <dsp/AudioProcessingBlock.h>
#include <ui/menus/AudioProcessorUIContextMenu.h>
#include <utils/XmlUtils.h>
#include <ui/interaction/DragHandler.h>

//Forward declarations
class GraphEditor;

#define AudioProcessorUIPtr std::shared_ptr<AudioProcessorUI>

class AudioProcessorUI : public Component,
                         public ManagedArray
{
public:
    class Listener
    {
    public:
        virtual void onProcessorClicked(AudioProcessorUI* processor, const MouseEvent& e) = 0;
        virtual void onProcessorMoved(AudioProcessorUI* processor, const MouseEvent& e) = 0;
        virtual void onProcessorReleased(AudioProcessorUI* processor, const MouseEvent& e) = 0;
        virtual void onContextSelection(AudioProcessorUI* processor, AudioProcessorConextMenuItems selection) = 0;
    };

    AudioProcessorUI();
    AudioProcessorUI(DspObjectType type);
    ~AudioProcessorUI();
    void paint(Graphics& g) override;
    void mouseDown(const MouseEvent& e) override;
    void mouseDrag(const MouseEvent& e) override;
    void mouseUp(const MouseEvent& e) override;
    void handleRightClick();
    void updateNameAndReCenter(String name);
    
    void addInputNode();
    void addOutputNode();
    Array<Node*> getAllNodes();
    Node* getInputNode();
    Node* getOutputNode();
    
    void connectInput(AudioProcessorUI* connection);
    void connectFeedbackInput(AudioProcessorUI* connection);
    void connectOutput(AudioProcessorUI* connection);
    void disconnectInput(AudioProcessorUI* connection);
    void disconnectOutput(AudioProcessorUI* connection);
    Array<AudioProcessorUI*> getOutputConnections();
    Array<AudioProcessorUI*> getInputConnections();
    Array<AudioProcessorUI*> getFeedbackConnections();
    
    virtual void setUIParameters() {};

    void addExistingInput(AudioProcessorUI* outputProcessor);
    void addExistingOutput(AudioProcessorUI* inputProcessor);
    
    void addListener(AudioProcessorUI::Listener* listener);

    AudioProcessingBlockPtr getProcessingBlock();
    void setProcessingBlock(AudioProcessingBlockPtr processingBlock);

    IAudioProcessor* getAudioProcessor();
    DspObjectType getType();
    
    bool isReversed();
    void reverse();

    std::string getIdAsString();

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

    AudioProcessingBlockPtr processingBlock = nullptr;

    Array<AudioProcessorUI*> inputConnections;
    Array<AudioProcessorUI*> outputConnections;
    Array<AudioProcessorUI*> feedbackConnections;

    Label nameLabel;
    String name;

    DspObjectType type;

    bool reversed = false;

    AudioProcessorUIContextMenu contextMenu;

    Array<Listener*> listeners;

    DragHandler dragHandler = DragHandler(this);
};

template<class T>
inline T* AudioProcessorUI::getAudioProcessorAs()
{
    return dynamic_cast<T*>(getAudioProcessor());
}