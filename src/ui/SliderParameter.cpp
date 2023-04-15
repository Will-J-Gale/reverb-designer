/*
  ==============================================================================

    SliderParameter.cpp
    Created: 25 Sep 2020 8:27:41pm
    Author:  Will

  ==============================================================================
*/

#include <ui/SliderParameter.h>
#include "Constants.h"

SliderParameter::SliderParameter()
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

    setInterceptsMouseClicks(false, true);
}

void SliderParameter::addListener(Slider::Listener* listener)
{
    slider.addListener(listener);
}

void SliderParameter::sliderValueChanged(Slider* slider)
{
}

void SliderParameter::setValue(double value)
{
    this->value = value;
    slider.setValue(value, dontSendNotification);
}

double SliderParameter::getValue()
{
    return value;
}

void SliderParameter::setMin(double min)
{
    setMinAndMax(min, max);

}

void SliderParameter::setMax(double max)
{
    setMinAndMax(min, max);
}

void SliderParameter::setMinAndMax(double min, double max)
{
    this->min = min;
    this->max = max;
    slider.setRange(min, max, SLIDER_INTERVAL);
}

void SliderParameter::setTextSuffix(const String& suffix)
{
    slider.setTextValueSuffix(suffix);
}

Component* SliderParameter::getComponent()
{
	return &slider;
}
