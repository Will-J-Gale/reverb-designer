#include <ui/windows/AudioProcessorMacroWindow.h>

AudioProcessorMacroWindow::AudioProcessorMacroWindow(
    const String& name, 
    OnCloseCallback onCloseCallback,
    Colour backgroundColour, 
    int buttonsNeeded)
        : DocumentWindow (name, backgroundColour, buttonsNeeded)
{
    this->onCloseCallback = onCloseCallback;
    setBounds(0,0, MACRO_WINDOW_DEFAULT_WIDTH, MACRO_WINDOW_DEFAULT_HEIGHT);
    setResizable(MACRO_WINDOW_RESIZABLE, MACRO_WINDOW_CORNER_RESIZE);
    setVisible(MACRO_WINDOW_DEFAULT_VISIBLE);
    setUsingNativeTitleBar(MACRO_WINDOW_USE_NATIVE_WINDOW);
}

void AudioProcessorMacroWindow::closeButtonPressed()
{
    this->onCloseCallback();
}