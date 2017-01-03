#ifndef PRICING_BLACKSCHOLESINDEXMODEL_HPP
#define PRICING_BLACKSCHOLESINDEXMODEL_HPP


#include "../ModelGen.hpp"

class BlackScholesIndexModel : public ModelGen {
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
    BlackScholesIndexModel(int assetNb, PnlVect *spot, PnlVect *trend, PnlVect *volatility, PnlMat *choleskyCorr, RateModelGen **rateModel);
    virtual ~BlackScholesIndexModel();

    /**
     * Overriding virtual method
     */
    void Simulate(double t, double maturity, PnlMat *path, const PnlMat *past, int stepNb);
    void Simulate(double maturity, PnlMat *path, int stepNb);
};


#endif //PRICING_BLACKSCHOLESINDEXMODEL_HPP
