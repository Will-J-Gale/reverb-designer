/*
  ==============================================================================

    AudioProcessorFactory.h
    Created: 27 Sep 2020 8:20:28pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include <stdexcept>
#include <dsp/IAudioProcessor.h>
#include <utils/Constants.h>
#include <dsp/Gain.h>
#include <dsp/SimpleDelay.h>
#include <dsp/AudioFilter.h>
#include <dsp/DelayAPF.h>
#include <dsp/CombFilter.h>
#include <dsp/ModulatedDelay.h>
#include <dsp/NestedDelayAPF.h>

class AudioProcessorFactory
{
public:
    static IAudioProcessorPtr Generate(AudioProcessorNodeType type)
    {
        switch (type)
        {
            case AudioProcessorNodeType::Input:
                return nullptr;
            
            case AudioProcessorNodeType::Output:
                return nullptr;

            case AudioProcessorNodeType::Gain:
                return std::make_shared<Gain>();

            case AudioProcessorNodeType::Delay:
                return std::make_shared<SimpleDelay>();

            case AudioProcessorNodeType::Filter:
                return std::make_shared<AudioFilter>();

            case AudioProcessorNodeType::APF:
                return std::make_shared<DelayAPF>();

            case AudioProcessorNodeType::Comb:
                return std::make_shared<CombFilter>();

            case AudioProcessorNodeType::ModDelay:
                return std::make_shared<ModulatedDelay>();

            case AudioProcessorNodeType::NestedApf:
                return std::make_shared<NestedDelayAPF>();

            default:
                throw std::invalid_argument("Missing object type");
        }
    }
};