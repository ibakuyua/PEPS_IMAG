#ifndef PRICING_MONTECARLOPRICER_HPP
#define PRICING_MONTECARLOPRICER_HPP

#include "PricerGen.hpp"

#define NB_SAMPLE_DEFAULT 40000

class MonteCarloPricer : public PricerGen{
public:
    /**
     * Members
     */
    ModelGen *simuIndexModel; /**< To simulate Indexes */
    ModelGen *simuChangeModel; /**< To simulate change */
    int nbSample; /**< Number of sample for the monte carlo method */

    /**
     * Constructor
     */
    MonteCarloPricer(double maturity, PayOffFunction payOff, ModelGen *simuIndex, ModelGen *simuChange, int nbSample = NB_SAMPLE_DEFAULT);

    /**
     * Overriding methods
     */
    void price(double t, PnlMat *past, double &price, double &ic) const;

    void delta(double t, PnlMat *past, PnlVect *delta) const;

    /**
     * Private method
     */
private:
    /**
     * To simulate the payOff
     *
     * @param[in] past : the past values of underlying assets
     * @pram[in] t : actual time
     */
    double payOffSimulation(PnlMat *past, double t) const;
};


#endif //PRICING_MONTECARLOPRICER_HPP
