#ifndef PRICING_MODELGEN_HPP
#define PRICING_MODELGEN_HPP

#include <stddef.h>
#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "../RateModels/RateModelGen.hpp"
#include "../FinancialProducts/Asset/Asset.hpp"
#include "../FinancialProducts/Asset/AssetList.hpp"
#include "../Stats/StatsFactory.h"
#include <map>
#include <vector>

/**
 * ModelGen [abstract]
 *
 * Class which permit to simulate a path of a list of asset
 */
class ModelGen {
public:
    /**
     * Members
     */
    int assetNb; /// Nb of asset
    string name; /// The name of the model

    PnlVect *spot; /// List of underlying asset spots
    PnlVect *trend; /// List of underlying trend spots
    PnlVect *volatility; /// List of underlying asset volatilities
    PnlRng *rng; /// List of rng to randomize simulation

    map<Change,RateModelGen*> rateModels; /// map of rateModel (one for each underlying asset economies)

    vector<Change> associatedChanges; /// Associated changes for underlying asset

    /**
     * Constructor
     *
     * remarks : the list of asset has to be setted after with SetAssets
     * remarks : Generaly is done in the product constructor
     *
     * @param[in] assetNb : Number of asset for the model
     * @param[in] economyNb : Number of different change in the model
     * @param[in] rateModels : list of rate model (one for each change)
     * @param[in] name : the name of the model
     */
    ModelGen(int assetNb, int economyNb, RateModelGen **rateModels, string name);

    /**
     * Virtual methods
     */
    /**
     * SetAssets : Permit to set properly the list of asset
     *
     * @param[in] assets : the list of assets
     */
    virtual void SetAssets(AssetList *assets);
    /**
     * Print : permit to print the model information
     */
    virtual void Print();

    /**
     * Virtual methods [Pur]
     */
    /**
     * // TODO : Améliorer ce truc
     * GetParametersFromStats : permit to obtain parameter model easily
     *
     * @param[in] stats : the statistic measures
     * @param[out] trend : the trend of each asset
     * @param[out] volMatrix : the matrice of volatility sigma
     */
    virtual void GetParametersFromStats(StatsFactory *stats,PnlVect **trend, PnlMat **volMatrix) = 0;
    /**
     * Simulate : permit to simulate at the date t a path for each asset.
     *
     * @param[in] t : the date to simulate
     * @param[in] maturity : the maturity for the simulation
     * @param[out] path : the result of the simulation
     * @param[in] past : the past values (the last is S(t) the spot at the date t)
     * @param[in] stepNb : the number of step of constatation
     *
     */
    virtual void Simulate(double t, double maturity, PnlMat *path, const PnlMat *past, int stepNb) = 0;
    /**
     * Simulate : permit to simulate at the date 0 a path for each asset
     */
    virtual void Simulate(double maturity, PnlMat *path, int stepNb) = 0;
    /**
     * SimulateMarket : permit to simulate a market
     *
     * @param[in] maturity : the maturity for the simulation
     * @param[out] path : the result of the simulation
     * @param[in] stepNb : the number of step of constatation
     */
    virtual void SimulateMarket(double maturity, PnlMat *path, int stepNb) = 0;

    /**
     * Destructor
     */
    virtual ~ModelGen();
};


#endif //PRICING_MODELGEN_HPP
