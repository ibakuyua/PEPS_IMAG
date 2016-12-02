#include "BlackScholesIndexModel.hpp"


BlackScholesIndexModel::BlackScholesIndexModel(int assetNb, PnlVect *spot, PnlVect *trend, PnlVect *volatility,
                                               PnlMat *choleskyCorr)
        : ModelGen(assetNb, spot, trend, volatility){
    this->choleskyCorr = choleskyCorr;
}

void BlackScholesIndexModel::Simulate(double t, double maturity, PnlMat *path, PnlMat *past, int stepNb) {
// TODO
}

