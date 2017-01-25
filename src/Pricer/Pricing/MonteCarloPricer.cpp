#include "MonteCarloPricer.hpp"
#include "../FinancialProducts/PayOffs/PayOffFunctions.hpp"


MonteCarloPricer::MonteCarloPricer(double maturity, ModelGen *simuModel, int nbSample, int nbTimeStep)
        : PricerGen(maturity, simuModel, "Monte Carlo")
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
        estimation = payOff(path,parameters,simuModel->rateModels);
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
    //Get useful values
    //Maturity
    double T = maturity;
    //FRR
    double r = simuModel->rateModels[Change::EUR]->GetIntegralRate(t,T);
    //Samples number
    double M = nbSample;
    double discountFactor = exp(-r * (T-t));

    PnlVect *St = pnl_vect_new();
    pnl_mat_get_row(St, past, past->m - 1);

    PnlVect *payOffDiff = pnl_vect_create(delta->size);
    pnl_vect_set_zero(delta);
    // For each simulation
    for (int m = 0; m < nbSample; ++m) {
        // Simulation at t
        PayOffSimulationShiftedDiff(payOffDiff,past,t,payOff,parameters);
        for (int d = 0; d < delta->size; ++d)
            LET(delta,d) = GET(payOffDiff,d);
    }

    // Delta
    for (int d = 0; d < delta->size; ++d){
        LET(delta,d) *= (discountFactor / (M * GET(St,d)));
    }


    // Free
    pnl_vect_free(&St);
    pnl_vect_free(&payOffDiff);



}

//TODO : Implement this function
void MonteCarloPricer::PayOffSimulationShiftedDiff(PnlVect *payOffDiff, const PnlMat *past, double t, PayOffFunction payOff, PnlVect *parameters) const {

    //Useful values
    double T = maturity;
    int nbTimeSteps = nbTimeStep;
    double timeStep = T / nbTimeSteps;
    int D = simuModel->assetNb;
    // derivation step
    double h = 0.000001;
    double moins_h = -h;

    //PnlMat* pathShifted = pnl_mat_create(nbTimeSteps + 1, D);
    PnlMat* pathShifted = pnl_mat_create_from_zero(path->m,path->n);

    //Simulation
    if (past == NULL || t == 0) {
        simuModel->Simulate(T,path,nbTimeSteps);
    }
    else {
        simuModel->Simulate(t,T,path,past, nbTimeSteps);
    }

    //For each asset, we shift the trajectory
    for (int d = 0; d < D; d++) {
        //Simulation of the path +h
        simuModel->ShiftAsset(pathShifted,path,d,h,t,timeStep);
        double payOffShifted = payOff(pathShifted,parameters,simuModel->rateModels);
        //Simulation of the path -h
        simuModel->ShiftAsset(pathShifted,path,d,moins_h,t,timeStep);
        payOffShifted -= payOff(pathShifted,parameters,simuModel->rateModels);
        //Don't forget to divide by the derivation step
        LET(payOffDiff,d) = payOffShifted/(2*h);
    }
}


MonteCarloPricer::~MonteCarloPricer() {
    pnl_mat_free(&path);
}
