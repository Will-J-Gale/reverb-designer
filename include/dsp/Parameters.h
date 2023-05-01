#pragma once
#include <cstddef>

//Filter defaults
const double  DEFAULT_Q = 0.707;
const double  DEFAULT_FC = 1000.0;
const double  DEFAULT_GAIN = 0.0;

//Delay defaults
const double  DEFAULT_MIX = 0.5;
const double  DEFAULT_DELAY_TIME = 1.0;
const double DEFAULT_FEEDBACK = 0.0;

//Envelope
const double TLD_AUDIO_ENVELOPE_ANALOG_TC = -0.99967234081320612357829304641019;
const double DEFAULT_ATTACK_MS = 10.0;
const double DEFAULT_RELEASE_MS = 10.0;
const double DEFAULT_THRESHOLD_DB = 0.0;
const double DEFAULT_SENSITIVITY = 1.0;
const double DEFAULT_OUTPUT_MIN = 0.0;
const double DEFAULT_OUTPUT_MAX = 1.0;

//Modulated Delay
const double DEFAULT_MOD_DELAY_RATE = 0.0;
const double DEFAULT_MOD_DELAY_DEPTH = 1.0;
const double DEFAULT_MOD_DELAY_FEEDBACK = 0.0;

//Reverb Tank
const size_t NUM_BRANCHES = 4;
const size_t NUM_CHANNELS = 2;

//Tube Pre
const size_t NUM_TUBES = 5;

enum class BiquadAlgorithm
{
    DirectForm,
    DirectFormTransposed,
    CanonicalForm,
    CanonicalFormTransposed
};

enum class FilterType
{
    EMPTY,
    LPF1,
    HPF1,
    LPF2,
    HPF2,
    LPFButterworth,
    HPFButterworth,
    BPF,
    BSF,
    APF,
    HSF,
    LSF,
    PEQ,
    PEQConstant,
    GEQ,
    LWRLPF2,
    LWRHPF2,
};

enum BiquadCoefficients
{
    A0,
    A1,
    A2,
    B1,
    B2,
    C0,
    D0,
    numCoeffs
};

enum BiquadStateRegister
{
    x_z1,
    x_z2,
    y_z1,
    y_z2,
    numStates
};

enum class GeneratorWaveform
{
    kTriangle,
    kSin,
    kSaw
};

enum class AudioDetectorMode
{
    kPeak,
    kMS,
    kRMS
};

enum class ModulatedDelayAlgorithm
{
    kFlanger,
    kVibrato,
    kChorus
};

enum class ReverbDensity 
{
    Thick, 
    Sparse
};

enum class DynamicsProcessorType
{
    Compressor,
    Expander
};

enum class DistortionModel
{
    SoftClip,
    ArcTan,
    FuzzAsym
};

//User parameters

/*
    These structs hold variables that will be directly changed
    by user input in the GUI
*/

struct IAudioParameter
{

};

struct AudioFilterParameters : IAudioParameter
{
    FilterType filterType = FilterType::LPF1;
    double fc = DEFAULT_FC;
    double Q = DEFAULT_Q;
    double gain_dB = DEFAULT_GAIN;
};

struct FeedbackDelayParameters : IAudioParameter
{
    float mix = DEFAULT_MIX;
    float delayInSeconds = DEFAULT_DELAY_TIME;
    float feedback = DEFAULT_FEEDBACK;
};

struct OscillatorParameters : IAudioParameter
{
    GeneratorWaveform waveForm = GeneratorWaveform::kTriangle;
    double frequency = 0.0; 
};

struct SignalGenData : IAudioParameter
{
    SignalGenData() {}

    double normalOutput = 0.0;			///< normal
    double invertedOutput = 0.0;		///< inverted
    double quadPhaseOutput_pos = 0.0;	///< 90 degrees out
    double quadPhaseOutput_neg = 0.0;	///< -90 degrees out
};

struct AudioDetectorParameters : IAudioParameter
{
    double attackTimeInMs;
    double releaseTimeInMs;
    AudioDetectorMode detectMode = AudioDetectorMode::kPeak;
    bool detectDb = false;
    bool clampToUnityMax = true;
};

struct EnvelopeFollowerParameters : IAudioParameter
{
    double attackTimeInMs = DEFAULT_ATTACK_MS;
    double releaseTimeInMs = DEFAULT_RELEASE_MS;
    double thresholdDb = DEFAULT_THRESHOLD_DB;
    double sensitivity = DEFAULT_SENSITIVITY;
    double outputMin = DEFAULT_OUTPUT_MIN;
    double outputMax = DEFAULT_OUTPUT_MAX;
};

struct ModulatedDelayParameters : IAudioParameter
{
    ModulatedDelayAlgorithm algorithm = ModulatedDelayAlgorithm::kChorus;
    double lfoRate = DEFAULT_MOD_DELAY_RATE;
    double lfoDepth = DEFAULT_MOD_DELAY_DEPTH;
    double feedback = DEFAULT_MOD_DELAY_FEEDBACK;
};

struct SimpleDelayParameters : IAudioParameter
{
    double delayTimeInMs = 0.0;
    bool interpolate = false;
};

struct SimpleLPFParameters : IAudioParameter
{
    double g = 0.0;
};

struct CombFilterParameters : IAudioParameter
{
    double delayTimeInMs = 0.0;
    double RT60TimeInMs = 0.0;
    bool enableLpf = false;
    double lpfG = 0.0;
    bool interpolate = false;
};

struct DelayAPFParameters : IAudioParameter
{
    double delayTimeInMs = 0.0;
    double apfG = 0.0;
    bool enableLPF = false;
    double lpfG = 0.0;
    bool interpolate = false;

    bool enableLFO = false;
    double lfoRate = 0.0;
    double lfoDepth = 0.0;
    double lfoMaxModulationInMs = 0.0;
};

struct NestedDelayAPFParameters : IAudioParameter
{
    double outerDelayTimeInMs = 0.0;
    double innerDelayTimeMs = 0.0;
    double outerApfG = 0.0;
    double innerApfG = 0.0;

    double enableLPF = false;
    double lpfG = 0.0;

    bool enableLFO = false;
    double lfoRateHz = 0.0;
    double lfoDepth = 1.0;
    double lfoMaxModulationInMs = 0.0;
};

struct TwoBandShelvingFilterParameters : IAudioParameter
{
    double lowSelfFreq = 0.0;
    double lowShelfGainInDb = 0.0;
    double highSelfFreq = 0.0;
    double highShelfGainInDb = 0.0;
};

struct ReverbTankParameters : IAudioParameter
{
    ReverbDensity reverbDensity = ReverbDensity::Thick;

    //tweaker parmeters
    double apfDelayMaxInMs = 5.0;
    double apfDelayWeight = 1.0;
    double fixedDelayMaxInMs = 50.0;
    double fixedDelayWeight = 1.0;

    //Direct controls
    double preDelayTimeInMs = 0.0;
    double lpfG = 0.0; //dampening 0 - 1
    double rT = 0.0; //reverb time 0 - 1

    //Shelving filters
    double lowShelfFreq = 0.0;
    double lowShelfGain = 0.0;
    double highShelfFreq = 0.0;
    double highShelfGain = 0.0;

    double wetLevelDb = -3.0;
    double dryLevelDb = -3.0;
};

//My stuff
struct ModifiedReverbParameters : IAudioParameter
{
    //tweaker parmeters
    double apfDelayMaxInMs = 5.0;
    double apfDelayWeight = 1.0;
    double fixedDelayMaxInMs = 50.0;
    double fixedDelayWeight = 1.0;

    //Direct controls
    double preDelayTimeInMs = 0.0;
    double lpfG = 0.0; //dampening 0 - 1
    double rT = 0.0; //reverb time 0 - 1

    //Shelving filters
    double lowShelfFreq = 0.0;
    double lowShelfGain = 0.0;
    double highShelfFreq = 0.0;
    double highShelfGain = 0.0;

    double wetLevelDb = -3.0;
    double dryLevelDb = -3.0;

    double roomSize = 1.0;
    double tapDamping = 0.0;
};

struct LRFilterBankParameters : IAudioParameter
{
    double splitFrequency = 0.0;
};

struct LRFilterBankOutput : IAudioParameter
{
    double LFOut = 0.0;
    double HFOut = 0.0;
};

struct DynamicsProcessorParameters : IAudioParameter
{
    double ratio = 50.0;
    double thresholdDb = -10.0;
    double kneeWidthDb = 10;
    bool hardLimit = false;
    bool softKnee = true;
    bool enableSideChain = false;
    DynamicsProcessorType calculation = DynamicsProcessorType::Compressor;

    double attackTimeInMs = 0.0;
    double releaseTimeInMs = 0.0;
    double outputGainDb = 0.0;

    //Output
    double gainReduction = 0.0;
    double gainReductionDb = 0.0;
};

struct BitChrusherParameters : IAudioParameter
{
    double bitDepth = 4.0;
    double downSample = 1.0;
};

struct TriodeClassAParameters : IAudioParameter
{
    DistortionModel waveshaper = DistortionModel::FuzzAsym;
    double saturation = 1.0;
    double asymmetry = 0.0;
    double outputGain = 0.0;
    double invertOutput = true;
    double enableHPF = true;
    double enableLSF = false;
    double hpfFreq = 1.0;
    double lsfFreq = 80.0;
    double lsfGain = 0.0;
};

struct ClassATubePreParameters : IAudioParameter
{
    double inputLevelDb = 0.0;
    double saturation = 1.0;
    double asymmetry = 0.0;
    double outputLevelDb = 0.0;

    double lowShelfFc = 0.0;
    double lowShelfGain = 0.0;
    double highShelfFc = 0.0;
    double highShelfGain = 0.0;
};