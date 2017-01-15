#ifndef PRICING_MODELGEN_HPP
#define PRICING_MODELGEN_HPP

#include <stddef.h>
#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "../RateModels/RateModelGen.hpp"
#include "../FinancialProducts/Asset.hpp"
#include <map>
#include <vector>

class ModelGen {
public:
    /**
     * Members
     */
    int assetNb;

    PnlVect *spot; /// List of underlying asset spots
    PnlVect *trend; /// List of underlying trend spots
    PnlVect *volatility; /// List of underlying asset volatilities
    PnlRng *rng; /// List of rng to randomize simulation

    map<Change,RateModelGen*> rateModels; /// map of rateModel (one for each underlying asset economies)

    vector<Change> associatedChanges; /// Associated changes for underlying asset

    /**
     * Constructor
     */
    ModelGen(int assetNb, Asset **assets, int economyNb, RateModelGen **rateModel);

    /**
     * Virtual methods
     */
    virtual void Simulate(double t, double maturity, PnlMat *path, const PnlMat *past, int stepNb) = 0;
    virtual void Simulate(double maturity, PnlMat *path, int stepNb) = 0;
    virtual void SimulateMarket(PnlMat *path, double maturity, int stepNb) = 0;

    /**
     * Destructor
     */
    virtual ~ModelGen();
};


#endif //PRICING_MODELGEN_HPP
