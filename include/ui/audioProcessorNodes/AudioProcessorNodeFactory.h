/*
  ==============================================================================

    AudioProcessorNodeFactory.h
    Created: 21 Sep 2020 11:14:33pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include <stdexcept>
#include <utils/Constants.h>
#include <ui/audioProcessorNodes/AudioProcessorNode.h>
#include <ui/audioProcessorNodes/GainNode.h>
#include <ui/audioProcessorNodes/BiquadFilterNode.h>
#include <ui/audioProcessorNodes/AllPassFilterNode.h>
#include <ui/audioProcessorNodes/CombFilterNode.h>
#include <ui/audioProcessorNodes/Input.h>
#include <ui/audioProcessorNodes/Output.h>
#include <ui/audioProcessorNodes/SimpleDelayNode.h>
#include <ui/audioProcessorNodes/ModDelayNode.h>
#include <ui/audioProcessorNodes/NestedAPFNode.h>

class GraphEditor;

class AudioProcessorNodeFactory
{
public:
    static AudioProcessorNodePtr Generate(DspObjectType type)
    {
        switch (type)
        {
            case DspObjectType::Input:
                return std::make_shared<Input>(type);

            case DspObjectType::Output:
                return std::make_shared<Output>(type);

            case DspObjectType::Gain:
                return std::make_shared<GainNode>(type);

            case DspObjectType::Delay:
                return std::make_shared<SimpleDelayNode>(type);

            case DspObjectType::Filter:
                return std::make_shared<BiquadFilterNode>(type);

            case DspObjectType::APF:
                return std::make_shared<AllPassFilterNode>(type);

            case DspObjectType::Comb:
                return std::make_shared<CombFilterNode>(type);

            case DspObjectType::ModDelay:
                return std::make_shared<ModDelayNode>(type);

            case DspObjectType::NestedApf:
                return std::make_shared<NestedAPFNode>(type);

            default:
                throw std::invalid_argument("Missing object type");
        }
    }
};