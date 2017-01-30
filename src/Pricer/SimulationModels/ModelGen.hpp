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

    // This member permit to compute faster simulation
    PnlVect *spot; /// List of underlying asset spots
    PnlVect *trend; /// List of underlying trend spots
    PnlVect *volatility; /// List of underlying asset volatilities
    PnlRng *rng; /// List of rng to randomize simulation

    AssetList *assetList; /// List of assets
    map<Change,RateModelGen*> rateModels; /// map of rateModel (one for each underlying asset economies)

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
      * ShiftAsset : permit to shift the asset number d in a path
      *
      * @param[out] path_shifted : the new path
      * @param[in] path : the path to shift
      * @param[in] d : index of asset to shift
      * @param[in] h : the shift
      * @param[in] t : the current date
      * @param[in] timestep : the time step
      */
    virtual void ShiftAsset(PnlMat *path_shifted, const PnlMat *path,
                            int d, double h, double t, double timestep);
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
     * @param[in] simulationSchedule : in the case of different step
     *
     */
    virtual void Simulate(double t, double maturity, PnlMat *path, const PnlMat *past, int stepNb, PnlVect *simulationSchedule = NULL) = 0;
    /**
     * Simulate : permit to simulate at the date 0 a path for each asset
     */
    virtual void Simulate(double maturity, PnlMat *path, int stepNb, PnlVect *simulationScheduled = NULL) = 0;


    /**
     * SimulateMarket : permit to simulate a market
     *
     * remarks : simulate assetNb+1 asset, the last is the domestic free risk asset
     *
     * @param[in] maturity : the maturity for the simulation
     * @param[out] path : the result of the simulation
     * @param[in] stepNb : the number of step of constatation
     * @param[in] domesticChange : the domestic ZC change
     */
    virtual void SimulateMarket(double maturity, PnlMat *path, int stepNb, Change domesticChange) = 0;

    /**
     * Destructor
     */
    virtual ~ModelGen();
};


#endif //PRICING_MODELGEN_HPP
