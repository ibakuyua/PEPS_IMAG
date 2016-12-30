#ifndef PRICING_PRICER_HPP
#define PRICING_PRICER_HPP


#include "../SimulationModels/ModelGen.hpp"
#include "pnl/pnl_matvect.h"

// Pointeur vers une fonction payOff
typedef double(*PayOffFunction)(PnlMat *path);

class PricerGen {
public:
    /**
     * Members
     */
    double maturity; /**< Maturity of the instrument to price */
    PayOffFunction payOff; /**< The payOff of the instrument */

    /**
     * Constructor
     */
    PricerGen(double maturity, PayOffFunction payOff);

    /**
     * Virtual methods
     */
    virtual void price(double t, PnlMat *past, double &price, double &ic) const = 0;
    virtual void delta(double t, PnlMat *past, PnlVect *delta) const = 0;

};


#endif //PRICING_PRICER_HPP
