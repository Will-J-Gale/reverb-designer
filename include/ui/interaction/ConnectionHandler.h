/*
  ==============================================================================

    ConnectionHandler.h
    Created: 26 Oct 2020 9:31:34pm
    Author:  Will

  ==============================================================================
*/

#pragma once
class GraphEditor;
class PluginGraph;
class AudioProcessorNodeConnectorUI;

class ConnectionHandler
{
public:
    ConnectionHandler(){};
    void initialize(GraphEditor* graphEditor, PluginGraph* pluginGraph);
    void createConnection(AudioProcessorNodeConnectorUI* start, AudioProcessorNodeConnectorUI* end);
    void createFeedbackConnection(AudioProcessorNodeConnectorUI* start, AudioProcessorNodeConnectorUI* end);
    void deleteConnection(AudioProcessorNodeConnectorUI* nodeConnector);
    bool connectionExists(AudioProcessorNodeConnectorUI* start, AudioProcessorNodeConnectorUI* end);
    bool isCreatingFeedback(AudioProcessorNodeConnectorUI* start, AudioProcessorNodeConnectorUI* end);
    bool nodesAreCompatible(AudioProcessorNodeConnectorUI* start, AudioProcessorNodeConnectorUI* end);

private:
    GraphEditor* graphEditor;
    PluginGraph* pluginGraph;
};