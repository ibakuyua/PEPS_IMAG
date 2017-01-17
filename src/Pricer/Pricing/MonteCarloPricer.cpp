#include "MonteCarloPricer.hpp"


MonteCarloPricer::MonteCarloPricer(double maturity, ModelGen *simuModel, int nbSample, int nbTimeStep)
        : PricerGen(maturity, simuModel)
{
    this->nbSample = nbSample;
    this->simuModel = simuModel;
    this->nbTimeStep = nbTimeStep;
    this->path = pnl_mat_create_from_zero(nbTimeStep+1,simuModel->assetNb);
}

void MonteCarloPricer::Price(double t, PnlMat *past, double &price, double &ic,
                             PayOffFunction payOff, PnlVect *parameters) const {

    // TODO : question si r est stochastique ?
    // TODO : faire monte carlo avec reduction de variance
    // European free interest rate
    double discountFactor = exp(-simuModel->rateModels[Change::EUR]->GetIntegralRate(t, maturity));

    double estimation, espEstimation = 0, varEstimation = 0;

    for (int m = 0; m < nbSample; ++m) {
        simuModel->Simulate(t, maturity, path, past, nbTimeStep);
        estimation = payOff(path,parameters);
        espEstimation += estimation;
        varEstimation += estimation * estimation;
    }
    espEstimation /= (double)nbSample;
    varEstimation /= (double)nbSample;
    varEstimation = fabs(varEstimation - espEstimation * espEstimation);

    price = discountFactor * espEstimation;
    ic = 3.92 * discountFactor * sqrt(varEstimation / (double)nbSample);
}

void MonteCarloPricer::Delta(double t, PnlMat *past, PnlVect *delta,
                             PayOffFunction payOff, PnlVect *parameters) const {
    // TODO
}

MonteCarloPricer::~MonteCarloPricer() {
    pnl_mat_free(&path);
}
