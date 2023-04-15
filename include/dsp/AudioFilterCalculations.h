#pragma once
#include <dsp/Parameters.h>
#include <utils/Math.h>

namespace AudioFilterCalculations
{
    inline void calculateLPF1(double* coeffs, double fc, double fs, double Q)
    {
        double theta = (2.0 * Math::PI * fc) / fs;
        double gamma = cos(theta) / (1.0 + sin(theta));

        coeffs[A0] = (1.0 - gamma) / 2.0;
        coeffs[A1] = (1.0 - gamma) / 2.0;
        coeffs[A2] = 0.0;
        coeffs[B1] = -gamma;
        coeffs[B2] = 0.0;
        coeffs[C0] = 1.0;
        coeffs[D0] = 0.0;
    }

    inline void calculateLPF2(double* coeffs, double fc, double fs, double Q)
    {
        double theta = (2.0 * Math::PI * fc) / fs;
        double d = 1.0 / Q;

        double sinThetha = sin(theta);
        double dSinTheta = (d / 2.0) * (sinThetha);
        double beta = 0.5 * ((1.0 - dSinTheta) / (1.0 + dSinTheta));
        double gamma = (0.5 + beta) * cos(theta);

        coeffs[A0] = (0.5 + beta - gamma) / 2.0;
        coeffs[A1] = 0.5 + beta - gamma;
        coeffs[A2] = (0.5 + beta - gamma) / 2.0;;
        coeffs[B1] = -2.0 * gamma;
        coeffs[B2] = 2.0 * beta;
        coeffs[C0] = 1.0;
        coeffs[D0] = 0.0;
    }

    inline void calculateLPFButterworth(double* coeffs, double fc, double fs, double Q)
    {
        double C = 1.0/ tan((Math::PI * fc) / fs);
        double C2 = C * C;

        double a0 = 1.0/ (1.0+ (sqrt(2) * C) + C2);
        coeffs[A0] = a0;
        coeffs[A1] = 2.0* a0;
        coeffs[A2] = a0;
        coeffs[B1] = (2.0* a0) * (1.0- C2);
        coeffs[B2] = a0 * (1.0- (sqrt(2) * C) + C2);
        coeffs[C0] = 1.0;
        coeffs[D0] = 0.0;
    }

    inline void calculateHPF1(double* coeffs, double fc, double fs, double Q)
    {
        double theta = (2.0 * Math::PI * fc) / fs;
        double gamma = cos(theta) / (1.0+ sin(theta));

        coeffs[A0] = (1.0+ gamma) / 2;
        coeffs[A1] = -((1.0+ gamma) / 2);
        coeffs[A2] = 0.0;
        coeffs[B1] = -gamma;
        coeffs[B2] = 0.0;
        coeffs[C0] = 1.0;
        coeffs[D0] = 0.0;
    }

    inline void calculateHPF2(double* coeffs, double fc, double fs, double Q)
    {
        double theta = (2.0 * Math::PI * fc) / fs;
        double d = 1.0/ Q;

        double sinThetha = sin(theta);
        double dSinTheta = (d / 2) * (sinThetha);
        double beta = 0.5 * ((1.0- dSinTheta) / (1.0+ dSinTheta));
        double gamma = (0.5 + beta) * cos(theta);

        coeffs[A0] = (0.5 + beta + gamma) / 2.0;
        coeffs[A1] = -(0.5 + beta + gamma);
        coeffs[A2] = (0.5 + beta + gamma) / 2.0;;
        coeffs[B1] = -2.0 * gamma;
        coeffs[B2] = 2.0 * beta;
        coeffs[C0] = 1.0;
        coeffs[D0] = 0.0;
    }

    inline void calculateHPFButterworth(double* coeffs, double fc, double fs, double Q)
    {
        double C = 2.0/ tan((Math::PI * fc) / fs);
        double C2 = C * C;

        double a0 = 1.0/ (1.0+ (sqrt(2) * C) + C2);
        coeffs[A0] = a0;
        coeffs[A1] = -2.0* a0;
        coeffs[A2] = a0;
        coeffs[B1] = (2.0* a0) * (C2 - 1);
        coeffs[B2] = a0 * (1.0- (sqrt(2) * C) + C2);
        coeffs[C0] = 1.0;
        coeffs[D0] = 0.0;
    }

    inline void calculateBPF(double* coeffs, double fc, double fs, double Q)
    {
        double K = tan((Math::PI * fc) / fs);
        double K2 = (K * K);

        double delta = (K2* Q) + K + Q;

        coeffs[A0] = K / delta;
        coeffs[A1] = 0.0;
        coeffs[A2] = -K / delta;
        coeffs[B1] = (2*Q) * (K2 - 1);
        coeffs[B2] = ((K2 * Q) - K + Q) / delta;
        coeffs[C0] = 1.0;
        coeffs[D0] = 0.0;
    }

    inline void calculateBSF(double* coeffs, double fc, double fs, double Q)
    {
        double K = tan((Math::PI * fc) / fs);
        double K2 = (K * K);

        double delta = (K2 * Q) + K + Q;

        coeffs[A0] = Q * (K2 + 1);
        coeffs[A1] = (2.0 * Q) * (K2 + 1);
        coeffs[A2] = (Q * (K2 + 1)) / delta;
        coeffs[B1] = (2.0 * Q) * (K2 - 1);
        coeffs[B2] = ((K2 * Q) - K + Q) / delta;
        coeffs[C0] = 1.0;
        coeffs[D0] = 0.0;
    }

    inline void calculateAPF(double* coeffs, double fc, double fs, double Q)
    {
    }

    inline void calculateHSF(double* coeffs, double fc, double fs, double Q, double gain_dB)
    {
        double theta = (2.0 * Math::PI * fc) / fs;
        double u = pow(10.0, gain_dB / 20.0);
        double beta = (1.0+ u) / 4;
        double delta = beta * tan(theta / 2.0);
        double gamma = (1.0- delta) / (1.0+ delta);

        coeffs[A0] = (1.0+ gamma) / 2;
        coeffs[A1] = -((1.0+ gamma) / 2);
        coeffs[A2] = 0.0;
        coeffs[B1] = -gamma;
        coeffs[B2] = 0.0;
        coeffs[C0] = u - 1.0;
        coeffs[D0] = 1.0;
    }

    inline void calculateLSF(double* coeffs, double fc, double fs, double Q, double gain_dB)
    {
        double theta = (2.0 * Math::PI * fc) / fs;
        double u = pow(10.0, gain_dB / 20.0);
        double beta = 4 / (1.0+ u);
        double delta = beta * tan(theta / 2.0);
        double gamma = (1.0- delta) / (1.0+ delta);

        coeffs[A0] = (1.0- gamma) / 2;
        coeffs[A1] = (1.0- gamma) / 2;;
        coeffs[A2] = 0.0;
        coeffs[B1] = -gamma;
        coeffs[B2] = 0.0;
        coeffs[C0] = u - 1.0;
        coeffs[D0] = 1.0;
    }

    inline void calculatePEQ(double* coeffs, double fc, double fs, double Q, double gain_dB)
    {
        double theta = (2.0 * Math::PI * fc) / fs;
        double u = pow(10.0, gain_dB / 20.0);
        double zeta = 4.0 / (1.0+ u);

        double zetaTanTheta = zeta * tan(theta / (2.0* Q));

        double beta = 0.5 * ((1.0- zetaTanTheta) / (1.0+ zetaTanTheta));
        double gamma = (0.5 + beta) * cos(theta);

        coeffs[A0] = 0.5 - beta;
        coeffs[A1] = 0.0;
        coeffs[A2] = -(0.5 - beta);
        coeffs[B1] = -2.0 * gamma;
        coeffs[B2] = 2.0 * beta;
        coeffs[C0] = u - 1.0;
        coeffs[D0] = 1.0;
    }

    inline void calculatePEQConstant(double* coeffs, double fc, double fs, double Q, double gain_dB)
    {
        double K = tan((Math::PI * fc) / fs);
        double K2 = K * K;
        double V0 = pow(10.0, gain_dB / 20.0);
        double d0 = 1.0 + ((1.0/ Q) * K) + K2;
        double e0 = 1.0 + ((1.0/ (V0 * Q)) * K) + K2;

        double alpha = 1.0+ ((V0 / Q) * K) + K2;
        double beta = 2.0 * (K2 - 1.0);
        double gamma = 1.0 - ((V0 / Q) * K) + K2;
        double delta = 1.0 - ((1.0/ Q) * K) + K2;
        double eta = 1.0 - ((1.0/ (V0 * Q)) * K) + K2;

        double a0;
        double a1;
        double a2;
        double b1;
        double b2;

        if (gain_dB >= 0)
        {
            a0 = alpha / d0;
            a1 = beta / d0;
            a2 = gamma / d0;
            b1 = beta / d0;
            b2 = delta / d0;
        }
        else
        {
            a0 = d0 / e0;
            a1 = beta / e0;
            a2 = delta / e0;
            b1 = beta / e0;
            b2 = eta / e0;
        }

        coeffs[A0] = a0;
        coeffs[A1] = a1;
        coeffs[A2] = a2;
        coeffs[B1] = b1;
        coeffs[B2] = b2;
        coeffs[C0] = 1.0;
        coeffs[D0] = 0.0;
    }

    inline void calculateGEQ(double* coeffs, double fc, double fs, double Q, double gain_dB)
    {
    }

    inline void calcualteLWRLPF2(double* coeffs, double fc, double fs)
    {
        double omega = Math::PI * fc;
        double omega2 = omega * omega;
        double theta = omega / fs;
        double kappa = omega / tan(theta);
        double kappa2 = kappa * kappa;
        double delta = kappa2 + omega2 + (2 * kappa * omega);

        coeffs[A0] = omega2 / delta;
        coeffs[A1] = 2 * (omega2 / delta);
        coeffs[A2] = omega2 / delta;;
        coeffs[B1] = ((-2 * kappa2) + (2 * omega2)) / delta;
        coeffs[B2] = ((-2 * kappa * omega) + (kappa2) + (omega2)) / delta;
        coeffs[C0] = 1.0;
        coeffs[D0] = 0.0;
    }

    inline void calcualteLWRHPF2(double* coeffs, double fc, double fs)
    {
        double omega = Math::PI * fc;
        double omega2 = omega * omega;
        double theta = omega / fs;
        double kappa = omega / tan(theta);
        double kappa2 = kappa * kappa;
        double delta = kappa2 + omega2 + (2 * kappa * omega);

        coeffs[A0] = kappa2 / delta;
        coeffs[A1] = (-2 * kappa2) / delta;
        coeffs[A2] = kappa2 / delta;
        coeffs[B1] = ((-2 * kappa2) + (2 * omega2)) / delta;
        coeffs[B2] = ((-2 * kappa * omega) + (kappa2)+(omega2)) / delta;
        coeffs[C0] = 1.0;
        coeffs[D0] = 0.0;
    } 
}