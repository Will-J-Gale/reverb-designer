#pragma once
#include <memory>
#include <functional>
#include <JuceHeader.h>

#define ParameterPtr std::shared_ptr<Parameter>
#define OnParameterChangeCallback std::function<void()>

enum class ParameterType
{
    Boolean,
    Float,
    Double,
    Option,
};

struct OptionItem
{
    OptionItem(){};
    OptionItem(std::string name, int value)
    {
        this->name = name;
        this->value = value;
    }

    std::string name;
    int value;
};

class Parameter
{
public:
    Parameter(ParameterType type, std::string name)
    {
        this->type = type;
        this->name = name;
    }

    ParameterType getType(){ return type; }
    std::string getName(){ return name; }
    void setOnChangeCallback(OnParameterChangeCallback callback){ onChangeCallback = callback; }

protected:
    ParameterType type;
    std::string name;
    OnParameterChangeCallback onChangeCallback = nullptr;
};

class BooleanParameter : public Parameter
{
public:
    BooleanParameter(std::string name, bool value) : Parameter(ParameterType::Boolean, name)
    {
        this->value = value;
    };

    bool getValue() { return value; }
    void setValue(bool value) 
    { 
        this->value = value;

        if(onChangeCallback != nullptr)
            onChangeCallback();
    }

private:
    bool value;
};

class FloatParameter : public Parameter
{
public:
    FloatParameter(std::string name, float value) : Parameter(ParameterType::Float, name)
    {
        this->value = value;
    };
    float getValue() { return value; }
    void setValue(float value)
    { 
        this->value = value;

        if(onChangeCallback != nullptr)
            onChangeCallback();
    }

private:
    float value;
};

struct OptionParameter : public Parameter
{
public:
    OptionParameter(std::string name, Array<OptionItem> options, int initialValueIndex) : Parameter(ParameterType::Option, name)
    {
        this->options = options;
        this->value = options[initialValueIndex].value;
    };

    int getValue() { return value; }
    void setValue(int value)
    { 
        this->value = value;

        if(onChangeCallback != nullptr)
            onChangeCallback();
    }

    Array<OptionItem> getItems(){ return options; }
    OptionItem getInitialItem(){ return initialItem; }

private:
    int value;
    Array<OptionItem> options;
    OptionItem initialItem;
};

class DoubleParameter : public Parameter
{
public:
    DoubleParameter(std::string name, double value, double min, double max) : Parameter(ParameterType::Double, name)
    {
        this->value = value;
        this->min = min;
        this->max = max;
    };

    double getValue() { return value; }
    void setValue(double value)
    { 
        this->value = value;

        if(onChangeCallback != nullptr)
            onChangeCallback();
    }

    double getMin() { return min; }
    double getMax() { return max; }

private:
    double value;
    double min;
    double max;
};