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
    inline NodeUIPtr Generate(NodeInstance type)
    {
        switch (type)
        {
            case NodeInstance::Input:
                return std::make_shared<Input>();

            case NodeInstance::Output:
                return std::make_shared<Output>();

            case NodeInstance::Gain:
                return std::make_shared<GainNode>();

            case NodeInstance::Delay:
                return std::make_shared<SimpleDelayNode>();

            case NodeInstance::Filter:
                return std::make_shared<BiquadFilterNode>();

            case NodeInstance::APF:
                return std::make_shared<AllPassFilterNode>();

            case NodeInstance::Comb:
                return std::make_shared<CombFilterNode>();

            case NodeInstance::ModDelay:
                return std::make_shared<ModDelayNode>();

            case NodeInstance::NestedApf:
                return std::make_shared<NestedAPFNode>();
            
            case NodeInstance::Macro:
                return std::make_shared<AudioProcessorMacroNode>();

            default:
                throw std::invalid_argument("Missing object type");
        }
    }
};