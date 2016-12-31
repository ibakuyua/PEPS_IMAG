#ifndef PRICING_MODELGEN_HPP
#define PRICING_MODELGEN_HPP

#include <stddef.h>
#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "../RateModels/RateModelGen.hpp"

class ModelGen {
public:
    /**
     * Members
     */
    int assetNb;

    PnlVect *spot;
    PnlVect *trend;
    PnlVect *volatility;
    PnlRng *rng;

    RateModelGen *rateModel;

    /**
     * Constructor
     */
    ModelGen(int assetNb, PnlVect *spot, PnlVect *trend, PnlVect *volatility);

    /**
     * Virtual methods
     */
    virtual void Simulate(double t, double maturity, PnlMat *path, const PnlMat *past, int stepNb) = 0;

    /**
     * Destructor
     */
    ~ModelGen();
};


#endif //PRICING_MODELGEN_HPP
