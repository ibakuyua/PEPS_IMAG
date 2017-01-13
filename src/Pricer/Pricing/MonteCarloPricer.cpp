#include "MonteCarloPricer.hpp"


MonteCarloPricer::MonteCarloPricer(double maturity, ModelGen *simuModel, int nbSample, int nbTimeStep, PayOffFunction payOffFunction)
        : PricerGen(maturity, payOffFunction)
{
    this->nbSample = nbSample;
    this->simuModel = simuModel;
    this->nbTimeStep = nbTimeStep;
    this->path = pnl_mat_create_from_zero(nbTimeStep+1,simuModel->assetNb);
}

void MonteCarloPricer::price(double t, PnlMat *past, double &price, double &ic) const {

    // TODO : question si r est stochastique ?
    // European free interest rate
    double r_T = simuModel->rateModels[5]->GetRate(maturity);
    double discountFactor = exp(-simuModel->rateModels[5]->GetIntegralRate(t, maturity));

    double estimation, espEstimation = 0, varEstimation = 0;

    for (int m = 0; m < nbSample; ++m) {
        simuModel->Simulate(t, maturity, path, past, nbTimeStep);
        estimation = payOff(path);
        espEstimation += estimation;
        varEstimation += estimation * estimation;
    }
    espEstimation /= (double)nbSample;
    varEstimation /= (double)nbSample;
    varEstimation = exp(-2 * r_T * maturity) * fabs(varEstimation - espEstimation * espEstimation);

    price = discountFactor * espEstimation;
    ic = 3.92 * sqrt(varEstimation / (double)nbSample);
}

void MonteCarloPricer::delta(double t, PnlMat *past, PnlVect *delta) const {
    // TODO
}

MonteCarloPricer::~MonteCarloPricer() {
    pnl_mat_free(&path);
}
