#pragma once
#include <JuceHeader.h>
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
        virtual void onNodeConnectorLeftClick(NodeConnectorUI* node, const MouseEvent& e) = 0;
        virtual void onNodeConnectorRightClick(NodeConnectorUI* node, const MouseEvent& e) = 0;
        virtual void onNodeConnectorDrag(NodeConnectorUI* node, const MouseEvent& e) = 0;
        virtual void onNodeConnectorLeftRelease(NodeConnectorUI* node, const MouseEvent& e) = 0;
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
    Uuid _id;
    NodeConnectorType _nodeConnectorType = NodeConnectorType::AudioInput;
    Point<float> _position;
    NodeConnectorUI* _connectedTo = nullptr;
    GraphEditor* _parent = nullptr;
    NodeUI* _attachedNodeUI;
    Array<NodeConnectorUI::Listener*> _listeners;
};