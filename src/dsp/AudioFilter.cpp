#include <cmath>
#include <dsp/AudioFilter.h>
#include <dsp/AudioFilterCalculations.h>
#include <utils/Math.h>

AudioFilter::AudioFilter()
{
    parameters = MAKE_PARAMETERS({
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

    parameters->addOnChangeCallback(std::bind(&AudioFilter::onParametersChanged, this));
    onParametersChanged();
}

bool AudioFilter::reset(double sampleRate)
{
    this->sampleRate = sampleRate;
    biquad.reset(sampleRate);
    calculateCoefficients();

	return true;
}

double AudioFilter::process(double xn)
{
    return (coeffs[D0] * xn) + (coeffs[C0] * biquad.process(xn));
}

bool AudioFilter::canProcessAudioFrame()
{
	return false;
}

void AudioFilter::setSampleRate(double newSampleRate)
{
    sampleRate = newSampleRate;
}

void AudioFilter::onParametersChanged()
{
    calculateCoefficients();
}

void AudioFilter::calculateCoefficients()
{
    double fs = sampleRate;
    double fc = parameters->getParameterValueByName<double>("Freq");
    double Q = parameters->getParameterValueByName<double>("Q");
    double gain_dB = parameters->getParameterValueByName<double>("Gain");
    FilterType filterType = parameters->getParameterValueByName<FilterType>("FilterType");

    switch (filterType)
    {
        case FilterType::LPF1:
            AudioFilterCalculations::calculateLPF1(coeffs, fc, fs, Q);
            break;

        case FilterType::LPF2:
            AudioFilterCalculations::calculateLPF2(coeffs, fc, fs, Q);
            break;

        case FilterType::LPFButterworth:
            AudioFilterCalculations::calculateLPFButterworth(coeffs, fc, fs, Q);
            break;

        case FilterType::HPF1:
            AudioFilterCalculations::calculateHPF1(coeffs, fc, fs, Q);
            break;

        case FilterType::HPF2:
            AudioFilterCalculations::calculateHPF2(coeffs, fc, fs, Q);
            break;

        case FilterType::HPFButterworth:
            AudioFilterCalculations::calculateHPFButterworth(coeffs, fc, fs, Q);
            break;

        case FilterType::BPF:
            AudioFilterCalculations::calculateBPF(coeffs, fc, fs, Q);
            break;

        case FilterType::BSF:
            AudioFilterCalculations::calculateBSF(coeffs, fc, fs, Q);
            break;

        case FilterType::APF:
            AudioFilterCalculations::calculateAPF(coeffs, fc, fs, Q);
            break;

        case FilterType::HSF:
            AudioFilterCalculations::calculateHSF(coeffs, fc, fs, Q, gain_dB);
            break;

        case FilterType::LSF:
            AudioFilterCalculations::calculateLSF(coeffs, fc, fs, Q, gain_dB);
            break;

        case FilterType::PEQ:
            AudioFilterCalculations::calculatePEQ(coeffs, fc, fs, Q, gain_dB);
            break;
        
        case FilterType::PEQConstant:
            AudioFilterCalculations::calculatePEQConstant(coeffs, fc, fs, Q, gain_dB);
            break;

        case FilterType::GEQ:
            AudioFilterCalculations::calculateGEQ(coeffs, fc, fs, Q, gain_dB);
            break;

        case FilterType::LWRLPF2:
            AudioFilterCalculations::calcualteLWRLPF2(coeffs, fc, fs);
            break;
        case FilterType::LWRHPF2:
            AudioFilterCalculations::calcualteLWRHPF2(coeffs, fc, fs);
            break;
        
    }

    biquad.setCoefficients(coeffs);
}