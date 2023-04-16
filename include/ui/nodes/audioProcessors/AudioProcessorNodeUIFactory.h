/*
  ==============================================================================

    AudioProcessorNodeUIFactory.h
    Created: 21 Sep 2020 11:14:33pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include <stdexcept>
#include <utils/Constants.h>
#include <ui/nodes/audioProcessors/AudioProcessorNodeUI.h>
#include <ui/nodes/audioProcessors/GainNode.h>
#include <ui/nodes/audioProcessors/BiquadFilterNode.h>
#include <ui/nodes/audioProcessors/AllPassFilterNode.h>
#include <ui/nodes/audioProcessors/CombFilterNode.h>
#include <ui/nodes/audioProcessors/Input.h>
#include <ui/nodes/audioProcessors/Output.h>
#include <ui/nodes/audioProcessors/SimpleDelayNode.h>
#include <ui/nodes/audioProcessors/ModDelayNode.h>
#include <ui/nodes/audioProcessors/NestedAPFNode.h>
#include <ui/nodes/audioProcessors/AudioProcessorMacroNode.h>

class GraphEditor;

class AudioProcessorNodeUIFactory
{
public:
    static AudioProcessorNodeUIPtr Generate(AudioProcessorNodeType type)
    {
        switch (type)
        {
            // case AudioProcessorNodeType::Input:
            //     return std::make_shared<Input>(type);

            // case AudioProcessorNodeType::Output:
            //     return std::make_shared<Output>(type);

            case AudioProcessorNodeType::Gain:
                return std::make_shared<GainNode>(type);

            case AudioProcessorNodeType::Delay:
                return std::make_shared<SimpleDelayNode>(type);

            case AudioProcessorNodeType::Filter:
                return std::make_shared<BiquadFilterNode>(type);

            case AudioProcessorNodeType::APF:
                return std::make_shared<AllPassFilterNode>(type);

            case AudioProcessorNodeType::Comb:
                return std::make_shared<CombFilterNode>(type);

            case AudioProcessorNodeType::ModDelay:
                return std::make_shared<ModDelayNode>(type);

            case AudioProcessorNodeType::NestedApf:
                return std::make_shared<NestedAPFNode>(type);
            
            case AudioProcessorNodeType::Macro:
                return std::make_shared<AudioProcessorMacroNode>(type);

            default:
                throw std::invalid_argument("Missing object type");
        }
    }
};