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
#include <dsp/PassThrough.h>
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
                return std::make_shared<SimpleDelay>();

            case NodeInstance::Filter:
                return std::make_shared<AudioFilter>();

            case NodeInstance::APF:
                return std::make_shared<DelayAPF>();

            case NodeInstance::Comb:
                return std::make_shared<CombFilter>();

            case NodeInstance::ModDelay:
                return std::make_shared<ModulatedDelay>();

            case NodeInstance::NestedApf:
                return std::make_shared<NestedDelayAPF>();
            
            default:
                throw std::invalid_argument("Missing object type");
        }
    }
};