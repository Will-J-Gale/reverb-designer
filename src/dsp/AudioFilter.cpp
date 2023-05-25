#include <cmath>
#include <dsp/AudioFilter.h>
#include <dsp/AudioFilterCalculations.h>
#include <utils/Math.h>

AudioFilter::AudioFilter()
{
    _parameters = MAKE_PARAMETERS({
        std::make_shared<DoubleParameter>("Freq", DEFAULT_FC, 20.0, 20000.0),
        std::make_shared<DoubleParameter>("Q", DEFAULT_Q, 0.01, 1.0),
        std::make_shared<DoubleParameter>("Gain", DEFAULT_GAIN, -10.0, 10.0),
        std::make_shared<OptionParameter>(
            "FilterType", 
            std::vector<OptionItem> {
                OptionItem("LPF", (int)FilterType::LPF1),
                OptionItem("HPF", (int)FilterType::HPF1),
                OptionItem("HSF", (int)FilterType::HSF),
                OptionItem("LSF", (int)FilterType::LSF),
            }, 
            0
        )
    });

    _parameters->addOnChangeCallback(std::bind(&AudioFilter::onParametersChanged, this));
    onParametersChanged();
}

void AudioFilter::reset(double sampleRate)
{
    _sampleRate = sampleRate;
    _biquad.reset(sampleRate);
    calculateCoefficients();
}

double AudioFilter::process(double xn)
{
    return (_coeffs[D0] * xn) + (_coeffs[C0] * _biquad.process(xn));
}

bool AudioFilter::canProcessAudioFrame()
{
	return false;
}

void AudioFilter::setSampleRate(double sampleRate)
{
    _sampleRate = sampleRate;
}

void AudioFilter::onParametersChanged()
{
    calculateCoefficients();
}

void AudioFilter::calculateCoefficients()
{
    double fs = _sampleRate;
    double fc = _parameters->getParameterValueByName<double>("Freq");
    double Q = _parameters->getParameterValueByName<double>("Q");
    double gain_dB = _parameters->getParameterValueByName<double>("Gain");
    FilterType filterType = _parameters->getParameterValueByName<FilterType>("FilterType");

    switch (filterType)
    {
        case FilterType::LPF1:
            AudioFilterCalculations::calculateLPF1(_coeffs, fc, fs, Q);
            break;

        case FilterType::LPF2:
            AudioFilterCalculations::calculateLPF2(_coeffs, fc, fs, Q);
            break;

        case FilterType::LPFButterworth:
            AudioFilterCalculations::calculateLPFButterworth(_coeffs, fc, fs, Q);
            break;

        case FilterType::HPF1:
            AudioFilterCalculations::calculateHPF1(_coeffs, fc, fs, Q);
            break;

        case FilterType::HPF2:
            AudioFilterCalculations::calculateHPF2(_coeffs, fc, fs, Q);
            break;

        case FilterType::HPFButterworth:
            AudioFilterCalculations::calculateHPFButterworth(_coeffs, fc, fs, Q);
            break;

        case FilterType::BPF:
            AudioFilterCalculations::calculateBPF(_coeffs, fc, fs, Q);
            break;

        case FilterType::BSF:
            AudioFilterCalculations::calculateBSF(_coeffs, fc, fs, Q);
            break;

        case FilterType::APF:
            AudioFilterCalculations::calculateAPF(_coeffs, fc, fs, Q);
            break;

        case FilterType::HSF:
            AudioFilterCalculations::calculateHSF(_coeffs, fc, fs, Q, gain_dB);
            break;

        case FilterType::LSF:
            AudioFilterCalculations::calculateLSF(_coeffs, fc, fs, Q, gain_dB);
            break;

        case FilterType::PEQ:
            AudioFilterCalculations::calculatePEQ(_coeffs, fc, fs, Q, gain_dB);
            break;
        
        case FilterType::PEQConstant:
            AudioFilterCalculations::calculatePEQConstant(_coeffs, fc, fs, Q, gain_dB);
            break;

        case FilterType::GEQ:
            AudioFilterCalculations::calculateGEQ(_coeffs, fc, fs, Q, gain_dB);
            break;

        case FilterType::LWRLPF2:
            AudioFilterCalculations::calcualteLWRLPF2(_coeffs, fc, fs);
            break;
        case FilterType::LWRHPF2:
            AudioFilterCalculations::calcualteLWRHPF2(_coeffs, fc, fs);
            break;
    }

    _biquad.setCoefficients(_coeffs);
}