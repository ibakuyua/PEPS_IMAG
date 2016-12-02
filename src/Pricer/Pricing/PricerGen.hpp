#ifndef PRICING_PRICER_HPP
#define PRICING_PRICER_HPP


#include "../SimulationModels/ModelGen.hpp"

// Pointeur vers une fonction payOff
typedef double(*PayOffFunction)(PnlMat *path);

class PricerGen {
public:
    /**
     * Members
     */
    ModelGen *simuIndexModel;
    ModelGen *simuChangeModel;

    /**
     * Constructor
     */
    PricerGen(ModelGen *simuIndex, ModelGen *simuChange);

    /**
     * Virtual methods
     */
    virtual void price(double t, PnlMat *past, double &price, double &ic, PayOffFunction payOff) const = 0;
    virtual void delta(double t, PnlMat *past, PnlVect *delta, PayOffFunction payOff) const = 0;

};


#endif //PRICING_PRICER_HPP
