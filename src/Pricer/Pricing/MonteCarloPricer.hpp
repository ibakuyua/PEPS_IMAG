#ifndef PRICING_MONTECARLOPRICER_HPP
#define PRICING_MONTECARLOPRICER_HPP

#include "PricerGen.hpp"

#define NB_SAMPLE_DEFAULT 40000
#define NB_TIME_STEP_DEFAULT 100

class MonteCarloPricer : public PricerGen{
public:
    /**
     * Members
     */
    int nbSample; /**< Number of sample for the monte carlo method */
    int nbTimeStep; /**< Number of time step for the simulation*/
    PnlMat *path; /**< path for simulation : allocated just one time */

    /**
     * Constructor / Destructor
     */
    MonteCarloPricer(double maturity, ModelGen *simuModel, int nbSample = NB_SAMPLE_DEFAULT,
                     int nbTimeStep = NB_TIME_STEP_DEFAULT);
    virtual ~MonteCarloPricer();

    /**
     * Overriding methods
     */
    void price(double t, PnlMat *past, double &price, double &ic,
               PayOffFunction payOff, PnlVect *parameters = NULL) const;

    void delta(double t, PnlMat *past, PnlVect *delta,
               PayOffFunction payOff, PnlVect *parameters = NULL) const;
};


#endif //PRICING_MONTECARLOPRICER_HPP
