#include <ui/interaction/NodeConnectorInteractionHandler.h>
#include <ui/GraphEditor.h>

NodeConnectorInteractionHandler::NodeConnectorInteractionHandler(GraphEditor* graphEditor)
{
    this->graphEditor = graphEditor;
}

void NodeConnectorInteractionHandler::onNodeConnectorLeftClick(NodeConnectorUI* node, const MouseEvent& e)
{
    graphEditor->interactionState = InteractionState::DragConnection;
    graphEditor->clickedNodeConnector = node;
}

void NodeConnectorInteractionHandler::onNodeConnectorRightClick(NodeConnectorUI* node, const MouseEvent& e)
{
    graphEditor->connectionHandler.deleteConnection(node);
}

void NodeConnectorInteractionHandler::onNodeConnectorDrag(NodeConnectorUI* nodeConnector, const MouseEvent& e)
{
    auto pos = e.getScreenPosition();
    graphEditor->mousePosition = graphEditor->getLocalPoint(nullptr, pos);

    graphEditor->repaint();
}

void NodeConnectorInteractionHandler::onNodeConnectorLeftRelease(NodeConnectorUI* nodeConnecctor, const MouseEvent& e)
{
    auto* mouseUpNodeConnector = graphEditor->getNodeConnectorAtPosition(e.getScreenPosition());

    if (mouseUpNodeConnector)
    {
        auto* startNode = graphEditor->clickedNodeConnector->getType() == NodeConnectorType::AudioOutput ? graphEditor->clickedNodeConnector : mouseUpNodeConnector;
        auto* endNode = mouseUpNodeConnector->getType() == NodeConnectorType::AudioInput ? mouseUpNodeConnector : graphEditor->clickedNodeConnector;

        if (!graphEditor->connectionHandler.connectionExists(startNode, endNode) 
            && graphEditor->connectionHandler.nodesAreCompatible(startNode, endNode))
        {
            if (graphEditor->connectionHandler.isCreatingFeedback(startNode, endNode))
                graphEditor->connectionHandler.createFeedbackConnection(startNode, endNode);
            else
                graphEditor->connectionHandler.createConnection(startNode, endNode);
        }
    }

    graphEditor->clickedNodeConnector = nullptr;
    graphEditor->interactionState = InteractionState::None;

    graphEditor->repaint();
}

