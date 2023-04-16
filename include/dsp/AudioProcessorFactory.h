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
    static IAudioProcessorPtr Generate(NodeType type)
    {
        switch (type)
        {
            case NodeType::Input:
                return nullptr;
            
            case NodeType::Output:
                return nullptr;

            case NodeType::Gain:
                return std::make_shared<Gain>();

            case NodeType::Delay:
                return std::make_shared<SimpleDelay>();

            case NodeType::Filter:
                return std::make_shared<AudioFilter>();

            case NodeType::APF:
                return std::make_shared<DelayAPF>();

            case NodeType::Comb:
                return std::make_shared<CombFilter>();

            case NodeType::ModDelay:
                return std::make_shared<ModulatedDelay>();

            case NodeType::NestedApf:
                return std::make_shared<NestedDelayAPF>();

            default:
                throw std::invalid_argument("Missing object type");
        }
    }
};