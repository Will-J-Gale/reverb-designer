/*
  ==============================================================================

    AudioProcessorFactory.h
    Created: 27 Sep 2020 8:20:28pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include <stdexcept>
#include "IAudioProcessor.h"
#include "Constants.h"
#include "Gain.h"
#include "SimpleDelay.h"
#include "AudioFilter.h"
#include "DelayAPF.h"
#include "CombFilter.h"
#include "ModulatedDelay.h"
#include "NestedDelayAPF.h"

class AudioProcessorFactory
{
public:
    static IAudioProcessorPtr Generate(DspObjectType type)
    {
        switch (type)
        {
            case DspObjectType::Input:
                return nullptr;
            
            case DspObjectType::Output:
                return nullptr;

            case DspObjectType::Gain:
                return std::make_shared<Gain>();

            case DspObjectType::Delay:
                return std::make_shared<SimpleDelay>();

            case DspObjectType::Filter:
                return std::make_shared<AudioFilter>();

            case DspObjectType::APF:
                return std::make_shared<DelayAPF>();

            case DspObjectType::Comb:
                return std::make_shared<CombFilter>();

            case DspObjectType::ModDelay:
                return std::make_shared<ModulatedDelay>();

            case DspObjectType::NestedApf:
                return std::make_shared<NestedDelayAPF>();

            default:
                throw std::invalid_argument("Missing object type");
        }
    }
};