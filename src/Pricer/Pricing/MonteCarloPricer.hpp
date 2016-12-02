#ifndef PRICING_MONTECARLOPRICER_HPP
#define PRICING_MONTECARLOPRICER_HPP

#include "PricerGen.hpp"

#define NB_SAMPLE_DEFAULT 40000

class MonteCarloPricer : PricerGen{
public:
    /**
     * Members
     */
    int nbSample;

    /**
     * Constructor
     */
    MonteCarloPricer(ModelGen *simuIndex, ModelGen *simuChange, int nbSample = NB_SAMPLE_DEFAULT);

    /**
     * Overriding methods
     */
    void price(double t, PnlMat *past, double &price, double &ic, PayOffFunction payOff) const;

    void delta(double t, PnlMat *past, PnlVect *delta, PayOffFunction payOff) const;
};


#endif //PRICING_MONTECARLOPRICER_HPP
