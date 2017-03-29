#ifndef PRICING_MONTECARLOPRICER_HPP
#define PRICING_MONTECARLOPRICER_HPP

#include "PricerGen.hpp"

#define NB_SAMPLE_DEFAULT 40000
#define NB_TIME_STEP_DEFAULT 100
#define DISCRETISATION_STEP 0.1

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


    /**
     * Constructor 1
     *
     * @param[in] maturity : maturity of the model
     * @param[in] simuModel : the simulation model
     * @param[in] nbSample : the number of sample to estimate the expectation
     * @param[in] nbTimeStep : the number of time step
     * @param[in] discr : the step of discretization
     */
	MonteCarloPricer(double maturity, ModelGen *simuModel,
                     int nbTimeStep = NB_TIME_STEP_DEFAULT, int nbSample = NB_SAMPLE_DEFAULT,
					 double discr = DISCRETISATION_STEP);
    /**
      * Constructor 2
      *
      * @param[in] maturity : maturity of the model
      * @param[in] simuModel : the simulation model
      * @param[in] nbSample : the number of sample to estimate the expectation
      * @param[in] scheduledStep : in the case of different steps
      * @param[in] discr : the step discretization
      */
	MonteCarloPricer(double maturity, ModelGen *simuModel, PnlVect* scheduledStep,
					 int nbSample = NB_SAMPLE_DEFAULT, double discr = DISCRETISATION_STEP);
	virtual ~MonteCarloPricer();

    /**
     * Overriding methods
     */
	void Price(double t, PnlMat *past, double &price, double &ic,
               PayOffFunction payOff, PnlVect *parameters = NULL) const;

	void Delta(double t, PnlMat *past, PnlVect *delta, PnlVect *ic,
               PayOffFunction payOff, PnlVect *parameters = NULL) const;

	void PayOffSimulationShiftedDiff(PnlVect *payOffDiff, const PnlMat *past, double t, PayOffFunction payOff, PnlVect *parameters) const;

private:
    /**
     * Private member
     */
    PnlMat *path; /**< path for simulation : allocated just one time */
    PnlMat *pathShifted; /*! pathShifted for delta simulation : initialized one times only*/
    double h; /**< h : step for derivation*/
};


#endif //PRICING_MONTECARLOPRICER_HPP
