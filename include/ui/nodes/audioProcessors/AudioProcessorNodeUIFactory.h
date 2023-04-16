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
#include <ui/nodes/NodeUI.h>
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

namespace AudioProcessorNodeUIFactory
{
    inline NodeUIPtr Generate(NodeType type)
    {
        switch (type)
        {
            case NodeType::Input:
                return std::make_shared<Input>();

            case NodeType::Output:
                return std::make_shared<Output>();

            case NodeType::Gain:
                return std::make_shared<GainNode>(type);

            case NodeType::Delay:
                return std::make_shared<SimpleDelayNode>(type);

            case NodeType::Filter:
                return std::make_shared<BiquadFilterNode>(type);

            case NodeType::APF:
                return std::make_shared<AllPassFilterNode>(type);

            case NodeType::Comb:
                return std::make_shared<CombFilterNode>(type);

            case NodeType::ModDelay:
                return std::make_shared<ModDelayNode>(type);

            case NodeType::NestedApf:
                return std::make_shared<NestedAPFNode>(type);
            
            case NodeType::Macro:
                return std::make_shared<AudioProcessorMacroNode>(type);

            default:
                throw std::invalid_argument("Missing object type");
        }
    }
};