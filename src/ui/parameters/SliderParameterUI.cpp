#include <ui/parameters/SliderParameterUI.h>
#include <utils/Constants.h>

SliderParameterUI::SliderParameterUI()
{
    setBounds(0, 0, SLIDER_PARAMETER_WIDTH, SLIDER_HEIGHT);
    _slider.setBounds(SLIDER_X, 0, SLIDER_WIDTH, SLIDER_HEIGHT);
    //slider.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    _slider.setRange(_min, _max, SLIDER_INTERVAL);
    _slider.setSliderStyle(Slider::LinearBar);
    
    _name.setInterceptsMouseClicks(false, false);
    
    _minLabel.setText(std::to_string(_min), dontSendNotification);
    _maxLabel.setText(std::to_string(_max), dontSendNotification);
    
    _minLabel.setBounds(MIN_TEXT_X, 0, 30, 20);
    _maxLabel.setBounds(MAX_TEXT_X, 0, 30, 20);
    _name.setBounds(SLIDER_NAME_X, -2, 100, 20);

    addAndMakeVisible(_slider);
    //addAndMakeVisible(minLabel);
    //addAndMakeVisible(maxLabel);

    setInterceptsMouseClicks(true, true);
}

void SliderParameterUI::addListener(Slider::Listener* listener)
{
    _slider.addListener(listener);
}

void SliderParameterUI::sliderValueChanged(Slider* slider)
{
}

void SliderParameterUI::setValue(double value)
{
    _value = value;
    _slider.setValue(value, dontSendNotification);
}

double SliderParameterUI::getValue()
{
    return _value;
}

void SliderParameterUI::setMin(double min)
{
    setMinAndMax(min, _max);
}

void SliderParameterUI::setMax(double max)
{
    setMinAndMax(_min, max);
}

void SliderParameterUI::setMinAndMax(double min, double max)
{
    _min = min;
    _max = max;
    _slider.setRange(min, max, SLIDER_INTERVAL);
}

void SliderParameterUI::setTextSuffix(const String& suffix)
{
    _slider.setTextValueSuffix(suffix);
}

Component* SliderParameterUI::getComponent()
{
	return &_slider;
}

float SliderParameterUI::getVisibleWidth()
{
    return _slider.getX() + _slider.getWidth() + getNameTextWidth() + PARAMETER_PADDING;
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