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
    this->graphEditor = graphEditor;
    this->pluginGraph = pluginGraph;
}

void NodeUIInteractionHandler::onNodeClicked(NodeUI* node, const MouseEvent& e)
{
    if (!graphEditor->selectionHandler.contains(node))
    {
        graphEditor->selectionHandler.clear();
    }

    graphEditor->repaint();
}

void NodeUIInteractionHandler::onNodeMoved(NodeUI* node, const MouseEvent& e)
{
    auto newE = e.getEventRelativeTo(graphEditor);
    graphEditor->selectionHandler.moveItems(newE.getOffsetFromDragStart());

    graphEditor->repaint();
}

void NodeUIInteractionHandler::onNodeReleased(NodeUI* node, const MouseEvent& e)
{
    graphEditor->selectionHandler.updateItemPositions();
}

void NodeUIInteractionHandler::onNodeContextSelection(NodeUI* node, NodeContextMenuItems selection)
{
    if (selection == NodeContextMenuItems::Delete)
    {
        if (!graphEditor->selectionHandler.isEmpty())
            graphEditor->deleteSelectedProcessors();
        else
            deleteNode(node);
    }
    else if (selection == NodeContextMenuItems::Duplicate)
    {
        if (!graphEditor->selectionHandler.isEmpty())
            graphEditor->duplicateSelectedProcessors();
        else
            duplicateNode(node);
    }
    else if (selection == NodeContextMenuItems::Reverse)
    {
        if (!graphEditor->selectionHandler.isEmpty())
            graphEditor->reverseSelectedProcessors();
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

    graphEditor->repaint();
}
NodeUIPtr NodeUIInteractionHandler::createMacroNode(Point<int> position, String macroName)
{
    //Refactor this to remove passthroughNode as macro really shouldn't need an explicit AudioProcessorNodePtr
    auto passthroughNode = graphEditor->pluginGraph->generateProcessorNode(NodeInstance::Macro);
    auto macroNode = std::make_shared<AudioProcessorMacroNode>(graphEditor->pluginGraph, macroName, passthroughNode);
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

    NodeUIPtr nodeUI = std::make_shared<IO>(NodeInstanceToName.at(instance), channel, ioProcessorNode, instance, graphEditor->parent);
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

    auto passthroughNode = graphEditor->pluginGraph->generateProcessorNode(NodeInstance::Macro);
    NodeUIPtr nodeUI = std::make_shared<IO>(NodeInstanceToName.at(instance), channel, passthroughNode, instance, graphEditor->parent);
    nodeUI->setTopLeftPosition(position);
    
    if(instance != NodeInstance::Input)
        nodeUI->addInputConnector();

    if(instance != NodeInstance::Output)
        nodeUI->addOutputConnector();

    return nodeUI;
}


NodeUIPtr NodeUIInteractionHandler::createNode(NodeInstance instance, Point<int> position)
{
    auto node = graphEditor->pluginGraph->generateProcessorNode(instance);
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
    graphEditor->nodes.add(node);
    graphEditor->nodeConnectors.addArray(node->getAllNodeConnectors());
    graphEditor->addNodeConnectorListeners(node->getAllNodeConnectors());
    graphEditor->addAndMakeVisible(node.get());
}

void NodeUIInteractionHandler::deleteNode(NodeUI *nodeUIToDelete)
{
    Array<NodeConnectorUI *> nodeConnectors = nodeUIToDelete->getAllNodeConnectors();
    for (auto *nodeConnector : nodeConnectors)
    {
        graphEditor->connectionHandler.deleteConnection(nodeConnector);
        graphEditor->removeFromArray(graphEditor->nodeConnectors, nodeConnector);
    }

    if(nodeUIToDelete->getNodeInstance() == NodeInstance::Macro)
    {
        Array<NodeUIPtr> internalNodeUIs = static_cast<AudioProcessorMacroNode*>(nodeUIToDelete)->getInternalNodes();
        Array<AudioProcessorNode*> internalNodes = {static_cast<AudioProcessorNodeUI*>(nodeUIToDelete)->getProcessorNode().get()};

        for(NodeUIPtr nodeUI : internalNodeUIs)
        {
            auto audioProcessorNode = static_cast<AudioProcessorNodeUI*>(nodeUI.get());
            internalNodes.add(audioProcessorNode->getProcessorNode().get());
        }
        
        pluginGraph->deleteProcessorNodes(internalNodes);
    }
    else
    {
        auto audioProcessorNode = static_cast<AudioProcessorNodeUI*>(nodeUIToDelete);
        pluginGraph->deleteProcessorNode(audioProcessorNode->getProcessorNode().get());
    }
        
    graphEditor->removeFromArray(graphEditor->nodes, nodeUIToDelete);
}

void NodeUIInteractionHandler::duplicateNode(NodeUI *node)
{
    auto newProcessor = createNode(node->getNodeInstance(), node->getPosition() + Point<int>(DUPLICATE_OFFSET_X, DUPLICATE_OFFSET_Y));
    // auto parameters = node->getAudioParametersAsXml();
    // newProcessor->setAudioParametersFromXml(parameters);
    // delete parameters;
    initializeNode(newProcessor);
}
