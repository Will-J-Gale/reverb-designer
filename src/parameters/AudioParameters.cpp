#include <parameters/AudioParameters.h>

AudioParameters::AudioParameters(std::initializer_list<ParameterPtr> parameters)
{
    setParameters(parameters);
}

ParameterPtr AudioParameters::getParameterByName(std::string name)
{
    try
    {
        return nameToParameterMap.at(name);
    }
    catch(std::out_of_range& e)
    {
        throw std::runtime_error(name + " not in parameters");
    }
}

void AudioParameters::setParameters(std::initializer_list<ParameterPtr> parameters)
{
    this->parameters = parameters;

    for(ParameterPtr parameter : parameters)
    {
        nameToParameterMap.insert({parameter->getName(), parameter});
    }
}
Array<ParameterPtr> AudioParameters::getAllParameters()
{
    return parameters;
}
void AudioParameters::addOnChangeCallback(OnParameterChangeCallback callback)
{
    for(ParameterPtr parameter : parameters)
    {
        parameter->setOnChangeCallback(callback);
    }
}
