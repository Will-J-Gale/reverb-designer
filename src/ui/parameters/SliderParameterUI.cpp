#include <ui/parameters/SliderParameterUI.h>
#include <utils/Constants.h>

SliderParameterUI::SliderParameterUI()
{
    setBounds(0, 0, SLIDER_PARAMETER_WIDTH, SLIDER_HEIGHT);
    slider.setBounds(SLIDER_X, 0, SLIDER_WIDTH, SLIDER_HEIGHT);
    //slider.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    slider.setRange(min, max, SLIDER_INTERVAL);
    slider.setSliderStyle(Slider::LinearBar);
    
    name.setInterceptsMouseClicks(false, false);
    
    minLabel.setText(std::to_string(min), dontSendNotification);
    maxLabel.setText(std::to_string(max), dontSendNotification);
    
    minLabel.setBounds(MIN_TEXT_X, 0, 30, 20);
    maxLabel.setBounds(MAX_TEXT_X, 0, 30, 20);
    name.setBounds(SLIDER_NAME_X, -2, 100, 20);

    addAndMakeVisible(slider);
    //addAndMakeVisible(minLabel);
    //addAndMakeVisible(maxLabel);

    setInterceptsMouseClicks(true, true);
}

void SliderParameterUI::addListener(Slider::Listener* listener)
{
    slider.addListener(listener);
}

void SliderParameterUI::sliderValueChanged(Slider* slider)
{
}

void SliderParameterUI::setValue(double value)
{
    this->value = value;
    slider.setValue(value, dontSendNotification);
}

double SliderParameterUI::getValue()
{
    return value;
}

void SliderParameterUI::setMin(double min)
{
    setMinAndMax(min, max);
}

void SliderParameterUI::setMax(double max)
{
    setMinAndMax(min, max);
}

void SliderParameterUI::setMinAndMax(double min, double max)
{
    this->min = min;
    this->max = max;
    slider.setRange(min, max, SLIDER_INTERVAL);
}

void SliderParameterUI::setTextSuffix(const String& suffix)
{
    slider.setTextValueSuffix(suffix);
}

Component* SliderParameterUI::getComponent()
{
	return &slider;
}

float SliderParameterUI::getVisibleWidth()
{
    return slider.getX() + slider.getWidth() + getNameTextWidth() + PARAMETER_PADDING;
}

// #warning DELETE
// void SliderParameterUI::mouseDown(const MouseEvent& e)
// {
//     if(e.mods.isLeftButtonDown())
//     {
//         auto parent = getParentComponent();
//         parent->mouseDown(e.getEventRelativeTo(parent));
//     }
// }

// void SliderParameterUI::mouseDrag(const MouseEvent& e)
// {
//     auto parent = getParentComponent();
//     parent->mouseDrag(e.getEventRelativeTo(parent));
//     e.getEventRelativeTo(parent);
// }