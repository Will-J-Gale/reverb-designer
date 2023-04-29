#pragma once
#include <memory>
#include <JuceHeader.h>
#include <utils/Constants.h>

namespace InputModals
{
    inline String runTextInputModal(String title, String message, String defaultText="")
    {
        auto asyncAlertWindow = std::make_unique<AlertWindow> (title, message, MessageBoxIconType::NoIcon);

        String textEditorName = "textEditor";
        asyncAlertWindow->addTextEditor(textEditorName, defaultText);
        asyncAlertWindow->addButton ("Create", 1, KeyPress (KeyPress::returnKey, 0, 0));
        asyncAlertWindow->runModalLoop(); 

        return asyncAlertWindow->getTextEditor(textEditorName)->getText();
    }
}