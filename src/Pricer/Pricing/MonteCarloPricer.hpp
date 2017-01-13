#ifndef PRICING_MONTECARLOPRICER_HPP
#define PRICING_MONTECARLOPRICER_HPP

#include "PricerGen.hpp"

#define NB_SAMPLE_DEFAULT 40000
#define NB_TIME_STEP_DEFAULT 10000

class MonteCarloPricer : public PricerGen{
public:
    /**
     * Members
     */
    ModelGen *simuModel; /**< To simulate underlying asset */
    int nbSample; /**< Number of sample for the monte carlo method */
    int nbTimeStep; /**< Number of time step for the simulation*/
    PnlMat *path; /**< path for simulation : allocated just one time */

    /**
     * Constructor / Destructor
     */
    MonteCarloPricer(double maturity, ModelGen *simuModel, int nbSample = NB_SAMPLE_DEFAULT,
                     int nbTimeStep = NB_TIME_STEP_DEFAULT, PayOffFunction payOffFunction = NULL);
    virtual ~MonteCarloPricer();

    /**
     * Overriding methods
     */
    void price(double t, PnlMat *past, double &price, double &ic) const;

    void delta(double t, PnlMat *past, PnlVect *delta) const;

};


#endif //PRICING_MONTECARLOPRICER_HPP
