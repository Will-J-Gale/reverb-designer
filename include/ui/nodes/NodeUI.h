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
        virtual void onProcessorClicked(NodeUI* processor, const MouseEvent& e) = 0;
        virtual void onProcessorMoved(NodeUI* processor, const MouseEvent& e) = 0;
        virtual void onProcessorReleased(NodeUI* processor, const MouseEvent& e) = 0;
        virtual void onContextSelection(NodeUI* processor, NodeUIConextMenuItems selection) = 0;
    };

    NodeUI(){};
    NodeUI(String name, NodeUIType type);
    ~NodeUI(){};

    void addInputConnector();
    void addOutputConnector();

    virtual void paint(Graphics& g) override;
    virtual void mouseDown(const MouseEvent& e) override;
    virtual void mouseDrag(const MouseEvent& e) override;
    virtual void mouseUp(const MouseEvent& e) override;
    virtual void handleRightClick();
    virtual void updateNameAndReCenter(String name);
    virtual void addListener(Listener* listener);
    virtual bool isReversed();
    virtual std::string getIdAsString();
    virtual void reverse(){};

protected:
    void initialize();
    Point<int> getCenterPosition();
    Uuid id;
    Label nameLabel;
    String name;
    NodeUIType type;
    bool reversed = false;
    NodeUIContextMenu contextMenu;
    Array<Listener*> listeners;
    DragHandler dragHandler = DragHandler(this);

    NodeConnectorUIPtr input;
    NodeConnectorUIPtr output;
};