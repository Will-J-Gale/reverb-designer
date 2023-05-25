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
    OptionItem(std::string option_name, int option_value)
    {
        name = option_name;
        value = option_value;
    }

    std::string name;
    int value;
};

class Parameter
{
public:
    Parameter(ParameterType type, std::string name)
    {
        _type = type;
        _name = name;
    }

    ParameterType getType(){ return _type; }
    std::string getName(){ return _name; }
    void setOnChangeCallback(OnParameterChangeCallback callback){ onChangeCallback = callback; }

    template<class T>
    T* asType()
    {
        return static_cast<T*>(this);
    }

protected:
    ParameterType _type;
    std::string _name;
    OnParameterChangeCallback onChangeCallback = nullptr;
};

class BooleanParameter : public Parameter
{
public:
    BooleanParameter(std::string name, bool value) : Parameter(ParameterType::Boolean, name)
    {
        _value = value;
    };

    bool getValue() { return _value; }
    void setValue(bool value) 
    { 
        _value = value;

        if(onChangeCallback != nullptr)
            onChangeCallback();
    }

private:
    bool _value;
};

class FloatParameter : public Parameter
{
public:
    FloatParameter(std::string name, float value) : Parameter(ParameterType::Float, name)
    {
        _value = value;
    };
    float getValue() { return _value; }
    void setValue(float value)
    { 
        _value = value;

        if(onChangeCallback != nullptr)
            onChangeCallback();
    }

private:
    float _value;
};

struct OptionParameter : public Parameter
{
public:
    OptionParameter(std::string name, std::vector<OptionItem> options, size_t initialValueIndex) : Parameter(ParameterType::Option, name)
    {
        _options = options;
        _value = options[initialValueIndex].value;
    };

    int getValue() { return _value; }
    void setValue(int value)
    { 
        _value = value;

        if(onChangeCallback != nullptr)
            onChangeCallback();
    }

    std::vector<OptionItem> getItems(){ return _options; }
    OptionItem getInitialItem(){ return initialItem; }

private:
    int _value;
    std::vector<OptionItem> _options;
    OptionItem initialItem;
};

class DoubleParameter : public Parameter
{
public:
    DoubleParameter(std::string name, double value, double min, double max) : Parameter(ParameterType::Double, name)
    {
        _value = value;
        _min = min;
        _max = max;
    };

    double getValue() { return _value; }
    void setValue(double value)
    { 
        _value = value;

        if(onChangeCallback != nullptr)
            onChangeCallback();
    }

    double getMin() { return _min; }
    double getMax() { return _max; }

private:
    double _value;
    double _min;
    double _max;
};