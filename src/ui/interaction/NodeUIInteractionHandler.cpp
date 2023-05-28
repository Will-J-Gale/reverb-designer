#include <dsp/AudioProcessorNode.h>
#include <dsp/PluginGraph.h>
#include <ui/interaction/NodeUIInteractionHandler.h>
#include <ui/nodes/audioProcessors/AudioProcessorNodeUI.h>
#include <ui/nodes/audioProcessors/AudioProcessorMacroNode.h>
#include <ui/GraphEditor.h>
#include <ui/interaction/ConnectionHandler.h>
#include <ui/menus/InputModals.h>
#include <ui/nodes/NodeUI.h>
#include <utils/Constants.h>
#include <utils/StorageManager.h>

void NodeUIInteractionHandler::initialize(GraphEditor* graphEditor, PluginGraph* pluginGraph)
{
    _graphEditor = graphEditor;
    _pluginGraph = pluginGraph;
}

void NodeUIInteractionHandler::onNodeClicked(NodeUI* node, const MouseEvent& e)
{
    if (!_graphEditor->_selectionHandler.contains(node))
    {
        _graphEditor->_selectionHandler.clear();
    }

    _graphEditor->repaint();
}

void NodeUIInteractionHandler::onNodeMoved(NodeUI* node, const MouseEvent& e)
{
    auto newE = e.getEventRelativeTo(_graphEditor);
    _graphEditor->_selectionHandler.moveItems(newE.getOffsetFromDragStart());

    _graphEditor->repaint();
}

void NodeUIInteractionHandler::onNodeReleased(NodeUI* node, const MouseEvent& e)
{
    _graphEditor->_selectionHandler.updateItemPositions();
}

void NodeUIInteractionHandler::onNodeContextSelection(NodeUI* node, NodeContextMenuItems selection)
{
    if (selection == NodeContextMenuItems::Delete)
    {
        if (!_graphEditor->_selectionHandler.isEmpty())
            _graphEditor->deleteSelectedProcessors();
        else
            deleteNode(node);
    }
    else if (selection == NodeContextMenuItems::Duplicate)
    {
        if (!_graphEditor->_selectionHandler.isEmpty())
            _graphEditor->duplicateSelectedProcessors();
        else
            duplicateNode(node);
    }
    else if (selection == NodeContextMenuItems::Reverse)
    {
        if (!_graphEditor->_selectionHandler.isEmpty())
            _graphEditor->reverseSelectedProcessors();
        else
            node->reverse();
    }
    else if (selection == NodeContextMenuItems::Save)
    {
        auto macroNode = static_cast<AudioProcessorMacroNode*>(node);
        String macroName = macroNode->getNodeName();
        XmlElement* xml = macroNode->toXml();
        String macrosPath = StorageManager::getStorageSubdirectoryPath(MACRO_DIR_NAME);
        macrosPath = macrosPath + "/" + macroName + SAVE_FILE_EXT;

        StorageManager::saveXML(macrosPath.toStdString(), xml->toString().toStdString());

        xml->deleteAllChildElements();
        delete xml;
    }

    _graphEditor->repaint();
}
NodeUIPtr NodeUIInteractionHandler::createMacroNode(Point<int> position, String macroName)
{
    //Refactor this to remove passthroughNode as macro really shouldn't need an explicit AudioProcessorNodePtr
    auto passthroughNode = _graphEditor->_pluginGraph->generateProcessorNode(NodeInstance::Macro);
    auto macroNode = std::make_shared<AudioProcessorMacroNode>(_graphEditor->_pluginGraph, macroName, passthroughNode);
    macroNode->setTopLeftPosition(position);
    macroNode->addInputConnector();
    macroNode->addOutputConnector();

    return macroNode;
}

NodeUIPtr NodeUIInteractionHandler::createMacroNodeFromXml(XmlElement* xml)
{
    auto macroNode = createMacroNode(Point<int>(), std::string());
    macroNode->fromXml(xml);
    return macroNode;
}



NodeUIPtr NodeUIInteractionHandler::createIONode(NodeInstance instance, Point<int> position, AudioProcessorNodePtr ioProcessorNode, int channel)
{
    jassert(instance == NodeInstance::Input || instance == NodeInstance::Output);

    NodeUIPtr nodeUI = std::make_shared<IO>(NodeInstanceToName.at(instance), channel, ioProcessorNode, instance, _graphEditor->_parent);
    nodeUI->setTopLeftPosition(position);
    
    if(instance != NodeInstance::Input)
        nodeUI->addInputConnector();

    if(instance != NodeInstance::Output)
        nodeUI->addOutputConnector();

    return nodeUI;
}

NodeUIPtr NodeUIInteractionHandler::createIONode(NodeInstance instance, Point<int> position, int channel)
{
    jassert(instance == NodeInstance::Input || instance == NodeInstance::Output);

    auto passthroughNode = _graphEditor->_pluginGraph->generateProcessorNode(NodeInstance::Macro);
    NodeUIPtr nodeUI = std::make_shared<IO>(NodeInstanceToName.at(instance), channel, passthroughNode, instance, _graphEditor->_parent);
    nodeUI->setTopLeftPosition(position);
    
    if(instance != NodeInstance::Input)
        nodeUI->addInputConnector();

    if(instance != NodeInstance::Output)
        nodeUI->addOutputConnector();

    return nodeUI;
}


NodeUIPtr NodeUIInteractionHandler::createNode(NodeInstance instance, Point<int> position)
{
    auto node = _graphEditor->_pluginGraph->generateProcessorNode(instance);
    return createNode(instance, position, node);
}

NodeUIPtr NodeUIInteractionHandler::createNode(NodeInstance instance, Point<int> position, AudioProcessorNodePtr processorNode)
{
    jassert(instance != NodeInstance::Input && instance != NodeInstance::Output);

    NodeUIPtr nodeUI = std::make_shared<AudioProcessorNodeUI>(NodeInstanceToName.at(instance), instance, processorNode);

    dynamic_cast<AudioProcessorNodeUI*>(nodeUI.get())->setProcessorNode(processorNode);
    nodeUI->setTopLeftPosition(position);

    if(instance != NodeInstance::Input)
        nodeUI->addInputConnector();

    if(instance != NodeInstance::Output)
        nodeUI->addOutputConnector();

    return nodeUI;
}

void NodeUIInteractionHandler::initializeNode(NodeUIPtr node)
{
    node->addListener(this);
    _graphEditor->_nodes.add(node);
    _graphEditor->_nodeConnectors.addArray(node->getAllNodeConnectors());
    _graphEditor->addNodeConnectorListeners(node->getAllNodeConnectors());
    _graphEditor->addAndMakeVisible(node.get());
}

void NodeUIInteractionHandler::deleteNode(NodeUI *nodeUIToDelete, bool deleteAudioNodes)
{
    Array<NodeConnectorUI *> nodeConnectors = nodeUIToDelete->getAllNodeConnectors();
    for (auto *nodeConnector : nodeConnectors)
    {
        _graphEditor->_connectionHandler.deleteConnection(nodeConnector);
        _graphEditor->removeFromArray(_graphEditor->_nodeConnectors, nodeConnector);
    }

    Array<AudioProcessorNode*> nodesToDelete;

    if(nodeUIToDelete->getNodeInstance() == NodeInstance::Macro)
    {
        Array<NodeUIPtr> internalNodeUIs = static_cast<AudioProcessorMacroNode*>(nodeUIToDelete)->getInternalNodes();

        for(NodeUIPtr nodeUI : internalNodeUIs)
        {
            auto audioProcessorNode = static_cast<AudioProcessorNodeUI*>(nodeUI.get());
            nodesToDelete.add(audioProcessorNode->getProcessorNode().get());
        }
    }
    else
    {
        auto audioProcessorNode = static_cast<AudioProcessorNodeUI*>(nodeUIToDelete);
        nodesToDelete.add(audioProcessorNode->getProcessorNode().get());
    }
        
    if(deleteAudioNodes)
        _pluginGraph->deleteProcessorNodes(nodesToDelete);
        
    _graphEditor->removeFromArray(_graphEditor->_nodes, nodeUIToDelete);
}

void NodeUIInteractionHandler::duplicateNode(NodeUI* node)
{
    std::shared_ptr<NodeUI> duplicatedNode;
    auto nodeXml = node->toXml();

    //@TODO make createNode return NodeUI instread of AudioProcessorNodeUI
    if(node->getNodeInstance() == NodeInstance::Macro)
        duplicatedNode = createMacroNodeFromXml(nodeXml);
    else
    {
        duplicatedNode = createNode(node->getNodeInstance(), Point<int>());
        duplicatedNode->fromXml(nodeXml);
    }

    nodeXml->deleteAllChildElements();
    delete nodeXml;

    duplicatedNode->setTopLeftPosition(node->getPosition() + Point<int>(DUPLICATE_OFFSET_X, DUPLICATE_OFFSET_Y));
    initializeNode(duplicatedNode);
}
