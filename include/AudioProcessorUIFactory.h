/*
  ==============================================================================

    AudioProcessorUIFactory.h
    Created: 21 Sep 2020 11:14:33pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include <stdexcept>
#include "Constants.h"
#include "AudioProcessorUI.h"
#include "GainUI.h"
#include "FilterUI.h"
#include "AllPassFilterUI.h"
#include "CombFilterUI.h"
#include "Input.h"
#include "Output.h"
#include <SimpleDelayUI.h>
#include <ModDelayUI.h>
#include <NestedApfUI.h>

class GraphEditor;

class AudioProcessorUIFactory
{
public:
    static AudioProcessorUIPtr Generate(DspObjectType type)
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