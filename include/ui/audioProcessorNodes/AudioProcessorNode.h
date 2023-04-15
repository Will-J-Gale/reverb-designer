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
#include <ui/audioProcessorNodes/Node.h>
#include <utils/Constants.h>
#include <utils/ManagedArray.h>
#include <dsp/IAudioProcessor.h>
#include <dsp/AudioProcessingBlock.h>
#include <ui/menus/AudioProcessorUIContextMenu.h>
#include <utils/XmlUtils.h>
#include <ui/interaction/DragHandler.h>

//Forward declarations
class GraphEditor;

#define AudioProcessorNodePtr std::shared_ptr<AudioProcessorNode>

class AudioProcessorNode : public Component,
                         public ManagedArray
{
public:
    class Listener
    {
    public:
        virtual void onProcessorClicked(AudioProcessorNode* processor, const MouseEvent& e) = 0;
        virtual void onProcessorMoved(AudioProcessorNode* processor, const MouseEvent& e) = 0;
        virtual void onProcessorReleased(AudioProcessorNode* processor, const MouseEvent& e) = 0;
        virtual void onContextSelection(AudioProcessorNode* processor, AudioProcessorConextMenuItems selection) = 0;
    };

    AudioProcessorNode();
    AudioProcessorNode(DspObjectType type);
    ~AudioProcessorNode();
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
    
    void connectInput(AudioProcessorNode* connection);
    void connectFeedbackInput(AudioProcessorNode* connection);
    void connectOutput(AudioProcessorNode* connection);
    void disconnectInput(AudioProcessorNode* connection);
    void disconnectOutput(AudioProcessorNode* connection);
    Array<AudioProcessorNode*> getOutputConnections();
    Array<AudioProcessorNode*> getInputConnections();
    Array<AudioProcessorNode*> getFeedbackConnections();
    
    virtual void setUIParameters() {};

    void addExistingInput(AudioProcessorNode* outputProcessor);
    void addExistingOutput(AudioProcessorNode* inputProcessor);
    
    void addListener(AudioProcessorNode::Listener* listener);

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

    Array<AudioProcessorNode*> inputConnections;
    Array<AudioProcessorNode*> outputConnections;
    Array<AudioProcessorNode*> feedbackConnections;

    Label nameLabel;
    String name;

    DspObjectType type;

    bool reversed = false;

    AudioProcessorUIContextMenu contextMenu;

    Array<Listener*> listeners;

    DragHandler dragHandler = DragHandler(this);
};

template<class T>
inline T* AudioProcessorNode::getAudioProcessorAs()
{
    return dynamic_cast<T*>(getAudioProcessor());
}