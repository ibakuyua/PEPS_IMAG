#ifndef PRICING_PRICER_HPP
#define PRICING_PRICER_HPP


#include "../SimulationModels/ModelGen.hpp"
#include "pnl/pnl_matvect.h"

/// Definition of a payOffFunction handle
typedef double(*PayOffFunction)(PnlMat *path, PnlVect *parameters);

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
    ModelGen *simuModel; /**< To simulate underlying asset */
    double maturity; /**< Maturity of the instrument to Price */

    /**
     * Constructor / Destructor
     *
     * @param[in] maturity : the maturity of the products you want to Price
     * @param[in] simuModel : the simulation model
     */
    PricerGen(double maturity, ModelGen *simuModel);
    virtual ~PricerGen();

    /**
     * Virtual methods
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
