#include <parameters/AudioParameters.h>

AudioParameters::AudioParameters(std::vector<ParameterPtr> parameters)
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

void AudioParameters::setParameters(std::vector<ParameterPtr> parameters)
{
    this->parameters = parameters;
    nameToParameterMap.clear();

    for(ParameterPtr parameter : parameters)
    {
        nameToParameterMap.insert({parameter->getName(), parameter});
    }
}
std::vector<ParameterPtr>& AudioParameters::getAllParameters()
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
