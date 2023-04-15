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
#include "Constants.h"

//Forward declarations
class GraphEditor;
class AudioProcessorUI;

#define NodePtr std::shared_ptr<Node>

class Node : public Component
{
public:
    class Listener
    {
    public:
        virtual void onNodeLeftClick(Node* node, const MouseEvent& e) = 0;
        virtual void onNodeRightClick(Node* node, const MouseEvent& e) = 0;
        virtual void onNodeDrag(Node* node, const MouseEvent& e) = 0;
        virtual void onNodeLeftRelease(Node* node, const MouseEvent& e) = 0;
    };

    Node();
    Node(AudioProcessorUI* attachedProcessorUI);
    ~Node();

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

    void addListener(Node::Listener* listener);

    AudioProcessorUI* getAttachedProcessor();

private:
    Uuid id;
    NodeType type = NodeType::AudioInput;
    Point<float> position;
    Node* connectedTo = nullptr;
    GraphEditor* parent = nullptr;
    AudioProcessorUI* attachedProcessorUI;
    Array<Node::Listener*> listeners;
};