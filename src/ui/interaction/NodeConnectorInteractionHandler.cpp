#include <ui/interaction/NodeConnectorInteractionHandler.h>
#include <ui/GraphEditor.h>
#include <utils/HitTest.h>

NodeConnectorInteractionHandler::NodeConnectorInteractionHandler(GraphEditor* graphEditor)
{
    _graphEditor = graphEditor;
}

void NodeConnectorInteractionHandler::onNodeConnectorLeftClick(NodeConnectorUI* node, const MouseEvent& e)
{
    _graphEditor->_interactionState = InteractionState::DragConnection;
    _graphEditor->_clickedNodeConnector = node;
}

void NodeConnectorInteractionHandler::onNodeConnectorRightClick(NodeConnectorUI* node, const MouseEvent& e)
{
    _graphEditor->_connectionHandler.deleteConnection(node);
}

void NodeConnectorInteractionHandler::onNodeConnectorDrag(NodeConnectorUI* nodeConnector, const MouseEvent& e)
{
    auto pos = e.getScreenPosition();
    _graphEditor->_mousePosition = _graphEditor->getLocalPoint(nullptr, pos);

    _graphEditor->repaint();
}

void NodeConnectorInteractionHandler::onNodeConnectorLeftRelease(NodeConnectorUI* nodeConnecctor, const MouseEvent& e)
{
    auto* mouseUpNodeConnector = HitTest::getNodeConnectorAtPosition(e.getScreenPosition(), _graphEditor->getNodeConnectors());

    if (mouseUpNodeConnector)
    {
        auto* startNode = _graphEditor->_clickedNodeConnector->getType() == NodeConnectorType::AudioOutput ? _graphEditor->_clickedNodeConnector : mouseUpNodeConnector;
        auto* endNode = mouseUpNodeConnector->getType() == NodeConnectorType::AudioInput ? mouseUpNodeConnector : _graphEditor->_clickedNodeConnector;

        if (!_graphEditor->_connectionHandler.connectionExists(startNode, endNode) 
            && _graphEditor->_connectionHandler.nodesAreCompatible(startNode, endNode))
        {
            if (_graphEditor->_connectionHandler.isCreatingFeedback(startNode, endNode))
                _graphEditor->_connectionHandler.createFeedbackConnection(startNode, endNode);
            else
                _graphEditor->_connectionHandler.createConnection(startNode, endNode);
        }
    }

    _graphEditor->_clickedNodeConnector = nullptr;
    _graphEditor->_interactionState = InteractionState::None;

    _graphEditor->repaint();
}

