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
    static std::string getPreset(PresetType presetId)
    {
        switch (presetId)
        {
            case PresetType::APF:
                return BinaryData::APF_xml;
            case PresetType::FeedbackComb:
                return BinaryData::FeedbackComb_xml;
            case PresetType::FeedforwardComb:
                return BinaryData::FeedforwardComb_xml;
            case PresetType::StereoDelay:
                return BinaryData::StereoDelay_xml;
            case PresetType::SchroederReverb:
                return BinaryData::SchroederReverb_xml;
            case PresetType::SpinSemiconductorReverb:
                return BinaryData::SpinSemiconductorReverb_xml;
            case PresetType::Figure8Reverb:
                return BinaryData::Figure8Reverb_xml;
        }
    }
};