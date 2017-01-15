#ifndef PRICING_BLACKSCHOLESINDEXMODEL_HPP
#define PRICING_BLACKSCHOLESINDEXMODEL_HPP


#include "ModelGen.hpp"

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
     */
    BlackScholesModel(int assetNb, Asset **assets, PnlMat *choleskyCorr, int economyNb, RateModelGen **rateModel);
    virtual ~BlackScholesModel();

    /**
     * Overriding virtual method
     */
    void Simulate(double t, double maturity, PnlMat *path, const PnlMat *past, int stepNb);

    void SimulateMarket(PnlMat *path, double maturity, int stepNb);

    void Simulate(double maturity, PnlMat *path, int stepNb);
};


#endif //PRICING_BLACKSCHOLESINDEXMODEL_HPP
