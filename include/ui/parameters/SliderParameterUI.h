#pragma once
#include <JuceHeader.h>
#include <ui/parameters/BaseParameterUI.h>

class SliderParameterUI : public BaseParameterUI,
                        public Slider::Listener
{
public:
    SliderParameterUI();
    void addListener(Slider::Listener* listener);
    virtual void sliderValueChanged(Slider* slider) override;

    void setValue(double value);
    double getValue();

    void setMin(double min);
    void setMax(double max);
    void setMinAndMax(double min, double max);

    void setTextSuffix(const String& suffix);

    virtual Component* getComponent() override;
    virtual float getVisibleWidth() override;
    // For setting min/max manually
    void mouseDown(const MouseEvent& e) override;
    void mouseDrag(const MouseEvent& e) override;

private:
    double value = 0.0;
    double min = 0.0;
    double max = 1.0;
    Slider slider;
    Label minLabel;
    Label maxLabel;
};