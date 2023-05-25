#include <parameters/AudioParameters.h>

AudioParameters::AudioParameters(std::vector<ParameterPtr> parameters)
{
    setParameters(parameters);
}

ParameterPtr AudioParameters::getParameterByName(std::string name)
{
    try
    {
        return _nameToParameterMap.at(name);
    }
    catch(std::out_of_range& e)
    {
        throw std::runtime_error(name + " not in parameters");
    }
}

void AudioParameters::setParameters(std::vector<ParameterPtr> parameters)
{
    _parameters = parameters;
    _nameToParameterMap.clear();

    for(ParameterPtr parameter : parameters)
    {
        _nameToParameterMap.insert({parameter->getName(), parameter});
    }
}
std::vector<ParameterPtr>& AudioParameters::getAllParameters()
{
    return _parameters;
}
void AudioParameters::addOnChangeCallback(OnParameterChangeCallback callback)
{
    for(ParameterPtr parameter : _parameters)
    {
        parameter->setOnChangeCallback(callback);
    }
}
