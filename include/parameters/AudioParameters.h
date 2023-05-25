#pragma once
#include <JuceHeader.h>
#include <parameters/Parameter.h>

#define AudioParametersPtr std::shared_ptr<AudioParameters>
#define MAKE_PARAMETERS(...) std::make_shared<AudioParameters>(std::vector<ParameterPtr>__VA_ARGS__)

class AudioParameters
{
public:
    AudioParameters(){};
    AudioParameters(std::vector<ParameterPtr> parameters);
    ParameterPtr getParameterByName(std::string name);

    template<class T>
    T* getParameterByNameAsType(std::string name)
    {
        return getParameterByName(name)->asType<T>();
    }

    template<typename ValueType>
    ValueType getParameterValueByName(std::string name)
    {
        Parameter* parameter = getParameterByName(name).get();

        switch(parameter->getType())
        {
            case ParameterType::Boolean:
                return (ValueType)static_cast<BooleanParameter*>(parameter)->getValue();
            
            case ParameterType::Double:
                return (ValueType)static_cast<DoubleParameter*>(parameter)->getValue();
            
            case ParameterType::Option:
                return (ValueType)static_cast<OptionParameter*>(parameter)->getValue();
            
            case ParameterType::Float:
                return (ValueType)static_cast<FloatParameter*>(parameter)->getValue();
            
            default:
                throw std::runtime_error("Invalid paramter type");
        }
    }

    template<typename ValueType>
    void setParameterValueByName(std::string name, ValueType value)
    {
        Parameter* parameter = getParameterByName(name).get();

        switch(parameter->getType())
        {
            case ParameterType::Boolean:
                static_cast<BooleanParameter*>(parameter)->setValue(value);
                return;
            
            case ParameterType::Double:
                static_cast<DoubleParameter*>(parameter)->setValue(value);
                return;
            
            case ParameterType::Option:
                static_cast<OptionParameter*>(parameter)->setValue(value);
                return;

            case ParameterType::Float:
                static_cast<FloatParameter*>(parameter)->setValue(value);
                return;
            
            default:
                throw std::runtime_error("Invalid paramter type");
        }
    }

    void addOnChangeCallback(OnParameterChangeCallback callback);
    void setParameters(std::vector<ParameterPtr> parameters);
    std::vector<ParameterPtr>& getAllParameters();

private:
    std::map<std::string, ParameterPtr> _nameToParameterMap;
    std::vector<ParameterPtr> _parameters;
};
