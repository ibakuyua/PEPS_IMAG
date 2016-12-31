#ifndef PRICING_BLACKSCHOLESINDEXMODEL_HPP
#define PRICING_BLACKSCHOLESINDEXMODEL_HPP


#include "../ModelGen.hpp"

class BlackScholesIndexModel : ModelGen {
public:
    /**
     * Members
     */
    PnlMat *choleskyCorr;

    /**
     * Constructor
     */
    BlackScholesIndexModel(int assetNb, PnlVect *spot, PnlVect *trend, PnlVect *volatility, PnlMat *choleskyCorr);

    /**
     * Overriding virtual method
     */
    void Simulate(double t, double maturity, PnlMat *path, const PnlMat *past, int stepNb);
};


#endif //PRICING_BLACKSCHOLESINDEXMODEL_HPP
