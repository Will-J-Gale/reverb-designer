#include <dsp/PassThrough.h>

PassThrough::PassThrough()
{   
    _parameters = MAKE_PARAMETERS({});
}

void PassThrough::reset(double sampleRate)
{
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
    return _parameters;
}
