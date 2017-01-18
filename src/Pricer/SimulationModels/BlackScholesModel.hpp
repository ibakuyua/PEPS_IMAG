#ifndef PRICING_BLACKSCHOLESINDEXMODEL_HPP
#define PRICING_BLACKSCHOLESINDEXMODEL_HPP


#include "ModelGen.hpp"

/**
 * BlackScholesModel
 *
 * Class which permit to simulate assets with a blackScholes model
 */
class BlackScholesModel : public ModelGen {
public:
    /**
     * Members
     */
    PnlMat *choleskyCorr; /**< For the correlation between indexes */
    PnlVect *Gi_; /**< Gaussian vector */
    PnlVect *LGi_; /**< Multiplication between choleskyCorr and Gi*/
    PnlVect *St; /**< For the simulation*/

    /**
     * Constructor / Destructor
     *
     * @param[in] assetNb : the number of assets
     * @param[in] choleskyCorr : the volatility matrix in the order of list of assets
     * @param[in] economyNb : the number of economy
     * @param[in] rateModel : the list of rate models for each economy
     * @param[in] assets : the list of assets
     */
    BlackScholesModel(int assetNb, PnlMat *choleskyCorr, int economyNb, RateModelGen **rateModel, Asset **assets);
    virtual ~BlackScholesModel();

    /**
     * Overriding virtual method
     */

    /**
     * remarks : the dynamic of S_i (asset from the economy i)
     *           is for this simulation : dS_i(t) = S_i(t)(r_i(t)dt + sigma(t).dWt)`
     *           with r_i the free risk rate in the economy i
     */
    void Simulate(double t, double maturity, PnlMat *path, const PnlMat *past, int stepNb);
    void Simulate(double maturity, PnlMat *path, int stepNb);

    /*
     * remarks : the dynamic is dS(t) = S(t)(mu(t)dt + sigma(t).dWt)
     */
    void SimulateMarket(double maturity, PnlMat *path, int stepNb);

};


#endif //PRICING_BLACKSCHOLESINDEXMODEL_HPP
