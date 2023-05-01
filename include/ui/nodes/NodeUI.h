#pragma once
#include <vector>
#include <JuceHeader.h>
#include <ui/menus/NodeUIContextMenu.h>
#include <ui/nodes/NodeConnectorUI.h>
#include <ui/interaction/DragHandler.h>
#include <utils/Constants.h>
#include <utils/ManagedArray.h>
#include <utils/XmlUtils.h>

//Forward declarations
class GraphEditor;

#define NodeUIPtr std::shared_ptr<NodeUI>
#define NodeConnectorUIPtr std::shared_ptr<NodeConnectorUI>

class NodeUI :  public Component,
                public ManagedArray
{
public:
    class Listener
    {
    public:
        virtual void onNodeClicked(NodeUI* processor, const MouseEvent& e) = 0;
        virtual void onNodeMoved(NodeUI* processor, const MouseEvent& e) = 0;
        virtual void onNodeReleased(NodeUI* processor, const MouseEvent& e) = 0;
        virtual void onNodeContextSelection(NodeUI* processor, NodeUIConextMenuItems selection) = 0;
    };

    NodeUI(){};
    NodeUI(String name, NodeClass nodeClass, NodeInstance nodeInstance);
    ~NodeUI(){};

    void addInputConnector();
    void addOutputConnector();
    NodeConnectorUI* getInputConnector();
    NodeConnectorUI* getOutputConnector();
    Array<NodeConnectorUI*> getAllNodeConnectors();

    virtual void connectInput(NodeUI* connector) = 0;
    virtual void connectFeedbackInput(NodeUI* connector) = 0;
    virtual void connectOutput(NodeUI* connector) = 0;
    virtual void disconnectInput(NodeUI* connector) = 0;
    virtual void disconnectOutput(NodeUI* connector) = 0;

    Array<NodeUI*> getOutputConnections();
    Array<NodeUI*> getInputConnections();
    Array<NodeUI*> getFeedbackConnections();

    NodeClass getNodeClass();
    NodeInstance getNodeInstance();

    virtual void paint(Graphics& g) override;
    virtual void mouseDown(const MouseEvent& e) override;
    virtual void mouseDrag(const MouseEvent& e) override;
    virtual void mouseUp(const MouseEvent& e) override;
    virtual void handleRightClick();
    virtual void updateNameAndReCenter(String name);
    virtual void addListener(Listener* listener);
    virtual std::string getIdAsString();
    virtual void reverse();
    virtual bool isReversed();

protected:
    void initialize();
    Point<int> getCenterPosition();

    bool reversed = false;
    Uuid id;
    Label nameLabel;
    String name;
    NodeClass nodeClass;
    NodeInstance nodeInstance;
    NodeUIContextMenu contextMenu;
    Array<Listener*> listeners;
    DragHandler dragHandler = DragHandler(this);

    NodeConnectorUIPtr input;
    NodeConnectorUIPtr output;

    Array<NodeUI*> inputConnections;
    Array<NodeUI*> outputConnections;
    Array<NodeUI*> feedbackConnections; 
};