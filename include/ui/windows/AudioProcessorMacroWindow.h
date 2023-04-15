#pragma once
#include <JuceHeader.h>

class AudioProcessorMacroWindow : public DocumentWindow
{
public:
    AudioProcessorMacroWindow(const String& name, Colour backgroundColour, int buttonsNeeded);
    void closeButtonPressed() override
    {
        delete this;
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioProcessorMacroWindow)
};