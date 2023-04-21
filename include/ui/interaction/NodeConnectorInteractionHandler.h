#pragma once
#include <ui/nodes/NodeConnectorUI.h>

//Forward declarations
class GraphEditor;

class NodeConnectorInteractionHandler : public NodeConnectorUI::Listener
{
public:
    NodeConnectorInteractionHandler(GraphEditor* graphEditor);
    void onNodeConnectorLeftClick(NodeConnectorUI* nodeConnector, const MouseEvent& e) override;
    void onNodeConnectorRightClick(NodeConnectorUI* nodeConnector, const MouseEvent& e) override;
    void onNodeConnectorDrag(NodeConnectorUI* nodeConnector, const MouseEvent& e) override;
    void onNodeConnectorLeftRelease(NodeConnectorUI* nodeConnector, const MouseEvent& e) override;

private:
    GraphEditor* graphEditor;
};