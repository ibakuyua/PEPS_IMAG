#ifndef PRICING_PRICER_HPP
#define PRICING_PRICER_HPP


#include "../SimulationModels/ModelGen.hpp"
#include "pnl/pnl_matvect.h"

/// Definition of a payOffFunction handle
typedef double(*PayOffFunction)(PnlMat *path, PnlVect *parameters, map<Change,RateModelGen*> &rateModels);

/**
 * PricerGen[abstract]
 *
 * Represent a type of pricer (with simulation)
 */
class PricerGen {
public:
    /**
     * Members
     */
    int nbTimeStep; /**< Number of time step for the simulation*/
    PnlVect *scheduleSimulation;  /**< the schedule that manage the way simulating (giving brownian motion steps)  */
    ModelGen *simuModel; /**< To simulate underlying asset */
    string name; /**< Name of the type of pricer*/
    double maturity; /**< Maturity of the instrument to Price */

    /**
     * Constructor 1
     *
     * @param[in] maturity : the maturity of the products you want to Price
     * @param[in] simuModel : the simulation model
     * @param[in] name : the name of the pricer
     * @param[in] nbTimeStep : the number of time step for the simulation
     */
    PricerGen(double maturity, ModelGen *simuModel, string name, int nbTimeStep);
    /**
     * Constructor 2
     *
     * @param[in] maturity : the maturity of the products you want to Price
     * @param[in] simuModel : the simulation model
     * @param[in] name : the name of the pricer
     * @param[in] scheduledSteps : in the case of different step for the simulation
     */
    PricerGen(double maturity, ModelGen *simuModel, string name, PnlVect *scheduledSteps);
    virtual ~PricerGen();

    /**
     * Virtual Methods
     */
    /**
     * PrintPricer : permit to print the pricer information
     */
    virtual void PrintPricer();

    /**
     * Virtual methods[pur]
     */
    /**
     * Price : permit to valuate at the date t product which give a payOff flux at maturity
     *
     * @param[in] t : the date to price the product
     * @param[in] past : the past values if any (the last is the spot at t)
     * @param[out] price : the result of the price
     * @param[out] ic : the width of the confidance interval
     * @param[in] payOff : the payOff function
     * @param[in] parameters(optionnal) : the parameters for the payOff function
     */
    virtual void Price(double t, PnlMat *past, double &price, double &ic,
                       PayOffFunction payOff, PnlVect *parameters = NULL) const = 0;
    /**
     * Delta : permit to give the delta of a product
     *
     * remarks : Return the gradient of the price (one line is corresponding to one asset)
     *
     * @param[in] t : the date to price the product
     * @param[in] past : the past values if any (the last is the spot at t)
     * @param[out] delta : the deltas for the payoff product
     * @param[in] payOff : the payOff function
     * @param[in] parameters(optionnal) : the parameters for the payOff function
     */
    virtual void Delta(double t, PnlMat *past, PnlVect *delta,
                       PayOffFunction payOff, PnlVect *parameters = NULL) const = 0;

};


#endif //PRICING_PRICER_HPP
