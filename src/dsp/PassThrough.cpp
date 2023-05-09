#include <dsp/PassThrough.h>

PassThrough::PassThrough()
{   
    parameters = MAKE_PARAMETERS({});
}

bool PassThrough::reset(double sampleRate)
{
    return true;
}

double PassThrough::process(double xn)
{
    return xn;
}

bool PassThrough::canProcessAudioFrame()
{
    return true;
}
AudioParametersPtr PassThrough::getParameters()
{
    return parameters;
}
