/*
  ==============================================================================

    PresetFactory.h
    Created: 25 Oct 2020 11:36:18am
    Author:  Will

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <utils/BinaryData.h>
#include <utils/Constants.h>
class PresetFactory
{
public:
    static std::string getPreset(Presets presetId)
    {
        switch (presetId)
        {
            case Presets::APF:
                return BinaryData::APF_xml;
            case Presets::FeedbackComb:
                return BinaryData::FeedbackComb_xml;
            case Presets::FeedforwardComb:
                return BinaryData::FeedforwardComb_xml;
            case Presets::StereoDelay:
                return BinaryData::StereoDelay_xml;
            case Presets::SchroederReverb:
                return BinaryData::SchroederReverb_xml;
            case Presets::SpinSemiconductorReverb:
                return BinaryData::SpinSemiconductorReverb_xml;
            case Presets::Figure8Reverb:
                return BinaryData::Figure8Reverb_xml;
        }
    }
};