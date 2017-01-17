#ifndef PRICING_MONTECARLOPRICER_HPP
#define PRICING_MONTECARLOPRICER_HPP

#include "PricerGen.hpp"

#define NB_SAMPLE_DEFAULT 40000
#define NB_TIME_STEP_DEFAULT 100

/**
 * MonteCarloPricer
 *
 * Represent the classic monte carlo method to price a product
 */
class MonteCarloPricer : public PricerGen{
public:
    /**
     * Public members
     */
    int nbSample; /**< Number of sample for the monte carlo method */
    int nbTimeStep; /**< Number of time step for the simulation*/

    /**
     * Constructor / Destructor
     *
     * @param[in] maturity : maturity of the model
     * @param[in] simuModel : the simulation model
     * @param[in] nbSample : the number of sample to estimate the expectation
     * @param[in] nbTimeStep : the number of time step
     */
    MonteCarloPricer(double maturity, ModelGen *simuModel, int nbSample = NB_SAMPLE_DEFAULT,
                     int nbTimeStep = NB_TIME_STEP_DEFAULT);
    virtual ~MonteCarloPricer();

    /**
     * Overriding methods
     */
    void Price(double t, PnlMat *past, double &price, double &ic,
               PayOffFunction payOff, PnlVect *parameters = NULL) const;

    void Delta(double t, PnlMat *past, PnlVect *delta,
               PayOffFunction payOff, PnlVect *parameters = NULL) const;

private:
    /**
     * Private member
     */
    PnlMat *path; /**< path for simulation : allocated just one time */
};


#endif //PRICING_MONTECARLOPRICER_HPP
