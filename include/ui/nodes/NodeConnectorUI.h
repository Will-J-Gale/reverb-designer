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
class NodeUI;

#define NodePtr std::shared_ptr<NodeConnectorUI>

class NodeConnectorUI : public Component
{
public:
    class Listener
    {
    public:
        virtual void onNodeLeftClick(NodeConnectorUI* node, const MouseEvent& e) = 0;
        virtual void onNodeRightClick(NodeConnectorUI* node, const MouseEvent& e) = 0;
        virtual void onNodeDrag(NodeConnectorUI* node, const MouseEvent& e) = 0;
        virtual void onNodeLeftRelease(NodeConnectorUI* node, const MouseEvent& e) = 0;
    };

    NodeConnectorUI();
    NodeConnectorUI(NodeUI* attachedProcessorUI);
    ~NodeConnectorUI();

    void initialize();

    void paint(Graphics& g) override;
    void setPosition(float x, float y);
    void setPosition(Point<float> newPosition);

    void mouseDown(const MouseEvent& e) override;
    void mouseDrag(const MouseEvent& e) override;
    void mouseUp(const MouseEvent& e) override;

    void setType(NodeConnectorType type);
    NodeConnectorType getType();

    Uuid getId();

    void addListener(NodeConnectorUI::Listener* listener);

    NodeUI* getAttachedNodeUI();

private:
    Uuid id;
    NodeConnectorType type = NodeConnectorType::AudioInput;
    Point<float> position;
    NodeConnectorUI* connectedTo = nullptr;
    GraphEditor* parent = nullptr;
    NodeUI* attachedProcessorUI;
    Array<NodeConnectorUI::Listener*> listeners;
};