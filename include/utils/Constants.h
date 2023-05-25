#pragma once
#include <string>
#include <memory>
#include <JuceHeader.h>

// #define UNUSED()

//Plugin size
constexpr int PLUGIN_WIDTH = 1800;
constexpr int PLUGIN_HEIGHT = 900;
constexpr int DSP_UI_NAME_Y = 10;
const std::string BACKGROUND_COLOUR_STRING = "ff323232";
const juce::Colour BACKGROUND_COLOUR = Colour::fromString(BACKGROUND_COLOUR_STRING);

//Text
constexpr int TEXT_HEIGHT = 40;
const std::string TEXT_COLOUR = "ff000000";

//Parameters
constexpr int PARAMETER_X_OFFSET = 10;
constexpr int PARAMETER_Y_OFFSET = 30;
constexpr int PARAMETER_HEIGHT = 20;
constexpr int PARAMETER_NAME_OFFSET = 110;
constexpr int PARAMETER_WIDTH = 250;
constexpr int PARAMETER_NAME_WIDTH = 200;
constexpr int PARAMETER_NAME_HEIGHT = 20;
constexpr int PARAMETER_SPACING = 30;
constexpr int PARAMETER_PADDING = 10;
constexpr int PARAMETER_TEXT_WIDTH = 100;
constexpr int PARAMETER_MAX_SIZE = 80;

//Slider parameter
constexpr int SLIDER_WIDTH = 75;
constexpr int SLIDER_HEIGHT = 15;
constexpr int SLIDER_PARAMETER_WIDTH = 350;
constexpr int MIN_TEXT_X = 0;
constexpr int MAX_TEXT_X = MIN_TEXT_X + SLIDER_WIDTH + 10;
constexpr int SLIDER_NAME_X = SLIDER_WIDTH + 5;
constexpr int SLIDER_X = MIN_TEXT_X + 10;
constexpr int SLIDER_SPACING = 15;
constexpr double SLIDER_INTERVAL = 0.01;

//Number parameter
constexpr int NUMBER_WIDTH = 75;
constexpr int NUMBER_TEXT_GAP = 10;

//Combobox parameter
constexpr int COMBO_X = PARAMETER_X_OFFSET;
constexpr int COMBO_WIDTH = 80;
constexpr int COMBO_HEIGHT = 20;
constexpr int COMBO_TEXT_OFFSET = COMBO_X + COMBO_WIDTH;
constexpr int COMBO_TEXT_WIDTH = 80;

//Bool parameter
constexpr int BOOL_PARAMETER_WIDTH = 70;
constexpr int BOOL_PARAMETER_HEIGHT = 15;
constexpr int BOOL_PARAMETER_START_X = 30;

//Node connectors
const std::string CONNECTOR_COLOUR = "ffff0000";
const std::string CONNECTOR_OUTPUT_COLOUR = "ff990000";
constexpr float CONNECTOR_SIZE = 12.0;
constexpr float INPUT_CONNECTOR_SIZE = CONNECTOR_SIZE * 0.85;
constexpr float CONNECTOR_BOUND_SIZE = CONNECTOR_SIZE * 1.5;
constexpr float CONNECTOR_RADIUS = CONNECTOR_SIZE / 2;
constexpr int CONNECTOR_LINE_THICKNESS = 2;

//Connections
const std::string CONNECTION_COLOUR = "ff000000";
constexpr int CONNECTION_GAP = 25;

//Audio Processor
constexpr int NODE_MIN_HEIGHT = 20;
constexpr int NODE_WIDTH = 150;
constexpr int NODE_HEIGHT = 60;
constexpr size_t MAX_CHANNELS = 10;
const std::string NODE_COLOUR = "ff02FF3E0";
constexpr float AUDIO_PROCESSOR_CORNER_SIZE = 5.0;
constexpr float NODE_THICKNESS = 2.0;
constexpr double DEFAULT_SAMPLERATE = 44100;

//Connections
constexpr int CONNECTION_THICKNESS = 3;
constexpr int ARROW_SIZE = 15;

//NodeUI
constexpr int NODE_DEFAULT_WIDTH = 200;
constexpr int NODE_SCALE_PER_PARAMETER = 50;

//Input/Output
const std::string INPUT_TEXT = "Input";
const std::string OUTPUT_TEXT = "Output";
constexpr int IO_WIDTH = 75;
constexpr int IO_HEIGHT = 20;
constexpr int INPUT_START_X = 0;
constexpr int OUTPUT_START_X = 500;
constexpr int IO_START_Y = 200;
constexpr int IO_SPACING = 100;

//Gain
const std::string GAIN_NAME = "Gain";
constexpr int GAIN_WIDTH = 100;
constexpr int GAIN_HEIGHT = 60;
constexpr int GAIN_INPUT_WIDTH = 50;

//BiquadFilterNode constants
const std::string FILTER_NAME = "Filter";
constexpr int FILTER_WIDTH = 200;
constexpr int FILTER_HEIGHT = 90;
constexpr double DEFAULT_FREQUENCY = 1000.0;

//Simple Delay
const std::string DELAY_NAME = "Simple Delay";
const std::string DELAY_TIME_TEXT = "Time Ms";
constexpr int DELAY_WIDTH = 145;
constexpr int DELAY_HEIGHT = 60;
constexpr int DELAY_TIME_WIDTH = 150;
constexpr int DELAY_TIME_HEIGHT = 20;
constexpr int DELAY_INPUT_WIDTH = 40;

//All Pass Filter
const std::string APF_NAME = "APF";
constexpr int APF_WIDTH = 180;
constexpr int APF_HEIGHT = 170;
constexpr double APF_MIN_DELAY = 0.1;
constexpr double APF_MAX_DELAY = 200.0;
constexpr double APF_MIN_MOD_FREQ = 0.1;
constexpr double APF_MAX_MOD_FREQ = 200.0;
constexpr double APF_LFO_RATE_MIN = 0.0;
constexpr double APF_LFO_RATE_MAX = 20.0;

//Nested APF filter
constexpr int NESTED_APF_WIDTH = 180;
constexpr int NESTED_APF_HEIGHT = 200;
const std::string NESTED_APF_NAME = "Nested APF";

//Comb filter
constexpr int COMB_WIDTH = 150;
constexpr int COMB_HEIGHT= 110;
const std::string COMB_NAME = "Comb Filter";

//ModDelay
const std::string MOD_DELAY_NAME = "Mod Delay";
constexpr int MOD_DELAY_WIDTH = 190;
constexpr int MOD_DELAY_HEIGHT = 120;
constexpr double MOD_DELAY_LFO_RATE_MIN = 0.0;
constexpr double MOD_DELAY_LFO_RATE_MAX = 20.0;

//Misc
constexpr int DUPLICATE_OFFSET_X = 20;
constexpr int DUPLICATE_OFFSET_Y = 20;
const std::string SELECTION_AREA_COLOUR = "330000BB";
const std::string SELECTED_PROCESSOR_COLOUR = "FF0000BB";
constexpr float CORNER_ROUNDNESS = 5.0;
constexpr float SELECTION_THICKNESS = 2.0;
constexpr float SELECTION_SCALE = 1.2;

//XML
const String PLUGIN_STATE_TAG = "PluginState";
const String PROCESSOR_STATE_TAG = "ProcessorState";
const String NODE_TAG = "Node";
const String NAME_TAG = "Name";
const String TYPE_TAG = "Type";
const String VALUE_TAG = "Value";
const String PROCESSORS_TAG = "Processors";
const String AUDIO_PARAMETERS_TAG = "AudioParameters";
const String PARAMETER_TAG = "Parameter";
const String MIN_TAG = "MinValue";
const String MAX_TAG = "MaxValue";
const String OPTIONS_TAG = "Options";
const String NODE_CLASS_TAG = "NodeClass";
const String INSTANCE_TAG = "NodeInstance";
const String X_TAG = "X";
const String Y_TAG = "Y";
const String IS_REVERSED_TAG = "IsReversed";
const String ID_TAG = "Id";
const String INPUTS_TAG = "Inputs";
const String OUTPUTS_TAG = "Outputs";
const String INPUT_CONNECTIONS_TAG = "InputConnections";
const String OUTPUT_CONNECTIONS_TAG = "OutputConnections";
const String FEEDBACK_CONNECTIONS_TAG = "FeedbackConnections";

const String AUDIO_PARAMETERTS_TAG = "AudioParameters";
const String GAIN_TAG = "Gain";
const String DELAY_TIME_TAG = "DelayTime";
const String CUTOFF_TAG = "Cutoff";
const String FILTER_TYPE_TAG = "FilterType";
const String Q_TAG = "Q";
const String APF_G_TAG = "ApfG";
const String ENABLE_LFO_TAG = "EnableLfo";
const String LFO_DEPTH_TAG = "LfoDepth";
const String LFO_RATE_TAG = "LfoRate";
const String MAX_MODULATION_TAG = "LfoMaxModulation";
const String ENABLE_LPF_TAG = "EnableLpf";
const String LPF_G_TAG = "LpfG";
const String REVERB_TIME_TAG = "ReverbTime";
const String MOD_ALGORITHM_TAG = "ModAlgorithm";
const String FEEDBACK_TAG = "Feedback";
const String OUTER_APF_G_TAG = "OuterApfG";
const String OUTER_DELAY_TIME_TAG = "OuterDelayTime";
const String INNER_APF_G_TAG = "InnerApfG";
const String INNER_DELAY_TIME_TAG = "OuterDelayTime";
const String IO_CHANNEL_TAG = "Channel";

//Macro window
const int MACRO_WINDOW_DEFAULT_WIDTH = 1000;
const int MACRO_WINDOW_DEFAULT_HEIGHT = 500;
const bool MACRO_WINDOW_RESIZABLE = true;
const bool MACRO_WINDOW_CORNER_RESIZE = false;
const bool MACRO_WINDOW_DEFAULT_VISIBLE = true;
const bool MACRO_WINDOW_USE_NATIVE_WINDOW = true;
const String MACRO_DEFAULT_NAME = "Macro";
const String MACRO_MODAL_TITLE = "Create macro";
const String MACRO_MODAL_MESSAGE = "Enter name for macro";

//Main menu
constexpr int MENU_HEIGHT = 20;
const String SAVE_TEXT = "Save As";
const String LOAD_TEXT = "Load";
const String NEW_PROJECT_TEXT = "New";
const String FILE_CHOOSER_TEXT = "Choose file...";
const String FILE_MENU_TEXT = "File";
const String PRESET_MENU_TEXT = "Presets";
const String MACRO_MENU_TEXT = "Macros";

//Preset test
const String STEREO_DELAY_PRESET_TEXT = "Stereo Delay";
const String FEEDFORWARD_COMB_PRESET_TEXT = "Feedforward Comb";
const String FEEDBACK_COMB_PRESET_TEXT = "Feedback Comb";
const String APF_PRESET_TEXT = "All Pass Filter";
const String SCHROEDER_PRESET_TEXT = "Schroeder Reverb";
const String ALLPASS_LOOP_PRESET_TEXT = "Allpass loop";
const String FIGURE8_PRESET_TEXT = "Figure 8 Reverb";

//Fonts
constexpr int FONT_SIZE = 12.00f;

//Storage
const String FILE_CHOOSER_FILTER = "*.xml";
const String SAVE_FILE_EXT = ".xml";
const String MACRO_DIR_NAME = "Macros";

enum class NodeConnectorType
{
    AudioInput,
    AudioOutput,
    ControlInput,
    ControlOutput
};

enum class ConnectionType
{
    Normal,
    Feedback
};

enum class NodeInstance
{
    Macro = -3,
    Input = -2,
    Output = -1,
    EMPTY = 0,
    
    //Audio processor nodes
    Gain,
    Delay,
    Filter,
    APF,
    Comb,
    ModDelay,
    NestedAPF,
};

const std::map<NodeInstance, std::string> NodeInstanceToName = {
    {NodeInstance::Input, "Input"},
    {NodeInstance::Output, "Output"},
    {NodeInstance::Macro, "Macro"},
    {NodeInstance::Gain, "Gain"},
    {NodeInstance::Delay, "Delay"},
    {NodeInstance::Filter, "Filter"},
    {NodeInstance::APF, "All Pass Filter"},
    {NodeInstance::Comb, "Comb Filter"},
    {NodeInstance::NestedAPF, "Nested APF"},
    {NodeInstance::ModDelay, "Modulated Delay"},
};

enum class NodeClass
{
    IO,
    AudioProcessor,
    Macro,
    Control
};

enum class GraphEditorContextMenuItems
{
    //DSP Objects
    EMPTY = (int)NodeInstance::EMPTY,
    Gain = (int)NodeInstance::Gain,
    Delay = (int)NodeInstance::Delay,
    Filter = (int)NodeInstance::Filter,
    APF = (int)NodeInstance::APF,
    Comb = (int)NodeInstance::Comb,
    ModDelay = (int)NodeInstance::ModDelay,
    NestedApf = (int)NodeInstance::NestedAPF,
    Macro = (int)NodeInstance::Macro,

    //Actions
    Duplicate,
    Paste
};

enum class NodeContextMenuItems
{
    EMPTY,
    Delete,
    Reverse,
    Duplicate,
    Save
};

enum class InteractionState
{
    None,
    MoveProcessor,
    DragConnection,
    CreateSelection
};

enum class SliderInteraction
{
    TextEnter = -1,
    Slide = 0
};