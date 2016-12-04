#ifndef PRICING_GAMTAUXRATEMODEL_HPP
#define PRICING_GAMTAUXRATEMODEL_HPP


#include "RateModelGen.hpp"

#define A_DEFAULT 0.4
#define R_LONG_DEFAULT 0.05
#define SPREAD_DEFAULT 0.02
#define GAMMA_DEFAULT 0.01

class GamTauxRateModel : public RateModelGen {
public:
    /**
     * Members
     */
    double a;
    double r_inf;
    double spread;
    double gamma;
    /**
     * Constructor
     */
    GamTauxRateModel(double r_inf = R_LONG_DEFAULT, double spread = SPREAD_DEFAULT,
                     double gamma = GAMMA_DEFAULT, double a = A_DEFAULT);
    /**
     * Overriding virtual function
     */
    double GetRate(double maturity) const;
    /**
     * Methods
     */
    double G1(double T) const;
    double G2(double T) const;
};


#endif //PRICING_GAMTAUXRATEMODEL_HPP
