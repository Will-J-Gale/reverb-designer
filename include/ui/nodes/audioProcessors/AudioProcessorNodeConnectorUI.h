/*
  ==============================================================================

    Node.h
    Created: 10 Sep 2020 6:53:28pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <memory>
#include <utils/Constants.h>

//Forward declarations
class GraphEditor;
class AudioProcessorNodeUI;

#define NodePtr std::shared_ptr<AudioProcessorNodeConnectorUI>

class AudioProcessorNodeConnectorUI : public Component
{
public:
    class Listener
    {
    public:
        virtual void onNodeLeftClick(AudioProcessorNodeConnectorUI* node, const MouseEvent& e) = 0;
        virtual void onNodeRightClick(AudioProcessorNodeConnectorUI* node, const MouseEvent& e) = 0;
        virtual void onNodeDrag(AudioProcessorNodeConnectorUI* node, const MouseEvent& e) = 0;
        virtual void onNodeLeftRelease(AudioProcessorNodeConnectorUI* node, const MouseEvent& e) = 0;
    };

    AudioProcessorNodeConnectorUI();
    AudioProcessorNodeConnectorUI(AudioProcessorNodeUI* attachedProcessorUI);
    ~AudioProcessorNodeConnectorUI();

    void initialize();

    void paint(Graphics& g) override;
    void setPosition(float x, float y);
    void setPosition(Point<float> newPosition);

    void mouseDown(const MouseEvent& e) override;
    void mouseDrag(const MouseEvent& e) override;
    void mouseUp(const MouseEvent& e) override;

    void setType(NodeType type);
    NodeType getType();

    Uuid getId();

    void addListener(AudioProcessorNodeConnectorUI::Listener* listener);

    AudioProcessorNodeUI* getAttachedProcessorUI();

private:
    Uuid id;
    NodeType type = NodeType::AudioInput;
    Point<float> position;
    AudioProcessorNodeConnectorUI* connectedTo = nullptr;
    GraphEditor* parent = nullptr;
    AudioProcessorNodeUI* attachedProcessorUI;
    Array<AudioProcessorNodeConnectorUI::Listener*> listeners;
};