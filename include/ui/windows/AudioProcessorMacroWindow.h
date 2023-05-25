#pragma once
#include <functional>
#include <JuceHeader.h>
#include <utils/Constants.h>

using OnCloseCallback = std::function<void()>;

class AudioProcessorMacroWindow : public DocumentWindow
{
public:
    AudioProcessorMacroWindow(
        const String& name,
        OnCloseCallback onCloseCallback,
        Colour backgroundColour=BACKGROUND_COLOUR, 
        int buttonsNeeded=DocumentWindow::closeButton);

    void closeButtonPressed() override;

private:
    OnCloseCallback _onCloseCallback;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioProcessorMacroWindow)
};