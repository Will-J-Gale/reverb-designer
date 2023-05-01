#pragma once
#include <dsp/IAudioProcessor.h>
#include <dsp/Parameters.h>
#include <dsp/FeedbackDelay.h>
#include <dsp/LFO.h>
#include <parameters/AudioParameters.h>
#include <utils/Constants.h>

class ModulatedDelay : public IAudioProcessor
{
public:
    ModulatedDelay();
    virtual bool reset(double sampleRate) override;
    virtual double process(double xn) override;
    virtual bool canProcessAudioFrame() override;
    virtual AudioParametersPtr getParameters(); 
    void onParametersChanged();

private:
    AudioParametersPtr parameters = MAKE_PARAMETERS({
        std::make_shared<OptionParameter>(
            "Algorithm", 
            Array<OptionItem> {
                OptionItem("Flanger", (int)ModulatedDelayAlgorithm::kFlanger),
                OptionItem("Vibrato", (int)ModulatedDelayAlgorithm::kVibrato),
                OptionItem("Chorus", (int)ModulatedDelayAlgorithm::kChorus),
            }, 
            2
        ),
        std::make_shared<DoubleParameter>("LFO Rate", DEFAULT_MOD_DELAY_RATE, 0.0, 20.0),
        std::make_shared<DoubleParameter>("LFO Depth", DEFAULT_MOD_DELAY_DEPTH, 0.0, 1.0),
        std::make_shared<DoubleParameter>("Feedback", DEFAULT_MOD_DELAY_FEEDBACK, 0.0, 1.0),
    });
    FeedbackDelay delay;
    AudioParametersPtr delayParameters = nullptr;
    LFO lfo;
};