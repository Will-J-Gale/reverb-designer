/*
  ==============================================================================

    AudioProcessorUIFactory.h
    Created: 21 Sep 2020 11:14:33pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include <stdexcept>
#include <utils/Constants.h>
#include <ui/audioProcessorNodes/AudioProcessorNode.h>
#include <ui/audioProcessorNodes/GainUI.h>
#include <ui/audioProcessorNodes/FilterUI.h>
#include <ui/audioProcessorNodes/AllPassFilterUI.h>
#include <ui/audioProcessorNodes/CombFilterUI.h>
#include <ui/audioProcessorNodes/Input.h>
#include <ui/audioProcessorNodes/Output.h>
#include <ui/audioProcessorNodes/SimpleDelayUI.h>
#include <ui/audioProcessorNodes/ModDelayUI.h>
#include <ui/audioProcessorNodes/NestedApfUI.h>

class GraphEditor;

class AudioProcessorUIFactory
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
                return std::make_shared<GainUI>(type);

            case DspObjectType::Delay:
                return std::make_shared<SimpleDelayUI>(type);

            case DspObjectType::Filter:
                return std::make_shared<FilterUI>(type);

            case DspObjectType::APF:
                return std::make_shared<AllPassFilterUI>(type);

            case DspObjectType::Comb:
                return std::make_shared<CombFilterUI>(type);

            case DspObjectType::ModDelay:
                return std::make_shared<ModDelayUI>(type);

            case DspObjectType::NestedApf:
                return std::make_shared<NestedApfUI>(type);

            default:
                throw std::invalid_argument("Missing object type");
        }
    }
};