#include <math.h>
#include "GamTauxRateModel.hpp"


GamTauxRateModel::GamTauxRateModel(double r_inf, double spread, double gamma, double a)
        : r_inf(r_inf), spread(spread), gamma(gamma), a(a)
{
}

double GamTauxRateModel::G1(double T) const {
    return (1. - exp(-a * T)) / (a*T);
}

double GamTauxRateModel::G2(double T) const {
    double res = (1. - exp(-a * T));
    res *= res;
    res /= 4 * a *T;
    return res;
}

double GamTauxRateModel::GetRate(double maturity) const {
    return r_inf - spread * G1(maturity) + gamma * G2(maturity);
}
