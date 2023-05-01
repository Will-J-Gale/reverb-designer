#pragma once
#include <JuceHeader.h>
#include <ui/parameters/BaseParameter.h>

class SliderParameter : public BaseParameter,
                        public Slider::Listener
{
public:
    SliderParameter();
    void addListener(Slider::Listener* listener);
    virtual void sliderValueChanged(Slider* slider) override;

    void setValue(double value);
    double getValue();

    void setMin(double min);
    void setMax(double max);
    void setMinAndMax(double min, double max);

    void setTextSuffix(const String& suffix);

    virtual Component* getComponent() override;

private:
    double value = 0.0;
    double min = 0.0;
    double max = 1.0;
    Slider slider;
    Label minLabel;
    Label maxLabel;
};