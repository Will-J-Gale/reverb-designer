
#pragma once
#include <stdexcept>
#include <dsp/IAudioProcessor.h>
#include <dsp/PassThrough.h>
#include <utils/Constants.h>
#include <dsp/Gain.h>
#include <dsp/Delay.h>
#include <dsp/AudioFilter.h>
#include <dsp/DelayAPF.h>
#include <dsp/CombFilter.h>
#include <dsp/ModulatedDelay.h>
#include <dsp/NestedDelayAPF.h>

class AudioProcessorFactory
{
public:
    static IAudioProcessorPtr Generate(NodeInstance type)
    {
        switch (type)
        {
            case NodeInstance::Input:
                return std::make_shared<PassThrough>();
            
            case NodeInstance::Output:
                return std::make_shared<PassThrough>();

            case NodeInstance::Gain:
                return std::make_shared<Gain>();

            case NodeInstance::Delay:
                return std::make_shared<Delay>();

            case NodeInstance::Filter:
                return std::make_shared<AudioFilter>();

            case NodeInstance::APF:
                return std::make_shared<DelayAPF>();

            case NodeInstance::Comb:
                return std::make_shared<CombFilter>();

            case NodeInstance::ModDelay:
                return std::make_shared<ModulatedDelay>();

            case NodeInstance::NestedAPF:
                return std::make_shared<NestedDelayAPF>();
            
            default:
                return std::make_shared<PassThrough>();
        }
    }
};