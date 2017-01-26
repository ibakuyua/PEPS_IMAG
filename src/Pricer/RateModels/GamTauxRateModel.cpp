#include <math.h>
#include <stdexcept>
#include "GamTauxRateModel.hpp"


GamTauxRateModel::GamTauxRateModel(Change devise, double r_inf, double spread, double gamma, double a)
        : RateModelGen(devise), r_inf(r_inf), spread(spread), gamma(gamma), a(a)
{
}

double GamTauxRateModel::G1(double T) const {
    return (1. - exp(-a * T)) / (a*T);
}

double GamTauxRateModel::G2(double T) const {
    double res = (1. - exp(-a * T));
    res *= res;
    if (T == 0.)
        throw std::domain_error("Maturité du taux doit être > 0");
    res /= 4 * a *T;
    return res;
}

double GamTauxRateModel::GetRate(double maturity) const {
    return R(maturity);
}


double GamTauxRateModel::GetIntegralRate(double t, double maturity) const {
    // R(t,T) = (R(0,T)T - R(0,t)t)/(T-t)
    return R(maturity) * maturity - R(t) * t;
}


GamTauxRateModel::~GamTauxRateModel() {
}

double GamTauxRateModel::R(double T) const {
    return r_inf - spread * G1(T) + gamma * G2(T);
}


