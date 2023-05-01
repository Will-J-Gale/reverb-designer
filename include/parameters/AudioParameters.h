#pragma once
#include <JuceHeader.h>
#include <parameters/Parameter.h>

#define AudioParametersPtr std::shared_ptr<AudioParameters>
#define MAKE_PARAMETERS(...) std::make_shared<AudioParameters>(std::initializer_list<ParameterPtr>__VA_ARGS__)

class AudioParameters
{
public:
    AudioParameters(){};
    AudioParameters(std::initializer_list<ParameterPtr> parameters);
    ParameterPtr getParameterByName(std::string name);

    template<class T>
    T* getParameterByNameAsType(std::string name)
    {
        return static_cast<T*>(getParameterByName(name).get());
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
        }
    }

    template<class ParameterType, typename ValueType>
    void setParameterValueByName(std::string name, ValueType value)
    {
        ParameterPtr parameter = getParameterByName(name);
        ParameterType* parameterCast = static_cast<ParameterType*>(parameter.get());
        parameterCast->setValue(value);
    }

    void addOnChangeCallback(OnParameterChangeCallback callback);
    void setParameters(std::initializer_list<ParameterPtr> parameters);
    Array<ParameterPtr> getAllParameters();

private:
    std::map<std::string, ParameterPtr> nameToParameterMap;
    Array<ParameterPtr> parameters;
};
