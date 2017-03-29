#include "MonteCarloPricer.hpp"
#include "../FinancialProducts/PayOffs/PayOffFunctions.hpp"


MonteCarloPricer::MonteCarloPricer(double maturity, ModelGen *simuModel, int nbTimeStep, int nbSample, double discr)
        : PricerGen(maturity, simuModel, "Monte Carlo", nbTimeStep)
{

    this->nbSample = nbSample;
    this->simuModel = simuModel;
    this->path = pnl_mat_create_from_zero(nbTimeStep+1,simuModel->assetNb);
    this->pathShifted = pnl_mat_create(nbTimeStep+ 1, simuModel->assetNb);
    this->h = discr;
}

MonteCarloPricer::MonteCarloPricer(double maturity, ModelGen *simuModel, PnlVect *scheduledStep, int nbSample, double discr)
        : PricerGen(maturity, simuModel, "Monte Carlo", scheduledStep)
{
    this->nbSample = nbSample;
    this->simuModel = simuModel;
    this->path = pnl_mat_create_from_zero(nbTimeStep+1,simuModel->assetNb);
    this->pathShifted = pnl_mat_create(nbTimeStep+ 1, simuModel->assetNb);
    this->h = discr;
}

void MonteCarloPricer::Price(double t, PnlMat *past, double &price, double &ic,
                             PayOffFunction payOff, PnlVect *parameters) const {

    // European free interest rate
    double discountFactor = exp(-simuModel->rateModels[Change::EUR]->GetIntegralRate(t, maturity));

    double estimation, espEstimation = 0, varEstimation = 0;

    for (int m = 0; m < nbSample; ++m) {
        simuModel->Simulate(t, maturity, path, past, nbTimeStep, scheduleSimulation);
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

void MonteCarloPricer::Delta(double t, PnlMat *past, PnlVect *delta, PnlVect *ic,
                             PayOffFunction payOff, PnlVect *parameters) const {
    // Resize
    pnl_vect_resize(delta,simuModel->assetNb);
    pnl_vect_resize(ic, simuModel->assetNb);
    //Get useful values
    //Maturity
    double T = maturity;
    //FRR
    double r = simuModel->rateModels[Change::EUR]->GetIntegralRate(t,T);
    //Samples number
    double M = nbSample;
    double discountFactor = exp(-r);

    PnlVect *St = pnl_vect_new();
    pnl_mat_get_row(St, past, past->m - 1);

    PnlVect *payOffDiff = pnl_vect_create(delta->size);
    pnl_vect_set_zero(delta);
    // For each simulation
    for (int m = 0; m < nbSample; ++m) {
        // Simulation at t
        PayOffSimulationShiftedDiff(payOffDiff,past,t,payOff,parameters);
        for (int d = 0; d < delta->size; ++d) {
            LET(delta, d) += GET(payOffDiff, d);
            LET(ic, d) += GET(payOffDiff,d) * GET(payOffDiff,d);
        }
    }
    // Delta and ic
    for (int d = 0; d < delta->size; ++d){
        LET(delta,d) /= M;
        LET(ic,d) /= M;
        LET(ic,d) = fabs(GET(ic,d) - GET(delta,d) * GET(delta,d));
        LET(delta,d) *= (discountFactor / (GET(St,d) * 2 * h));
        LET(ic,d) = 3.92 * discountFactor * sqrt(GET(ic,d) / M) / (GET(St,d) * 2 * h);
    }

    // Free
    pnl_vect_free(&St);
    pnl_vect_free(&payOffDiff);
}

void MonteCarloPricer::PayOffSimulationShiftedDiff(PnlVect *payOffDiff, const PnlMat *past, double t, PayOffFunction payOff, PnlVect *parameters) const {

    //Useful values
    double T = maturity;
    int nbTimeSteps = nbTimeStep;


    double timeStep = T / nbTimeSteps;
    int D = simuModel->assetNb;
    double moins_h = -h;

    //Simulation
    if (t == 0) {
        simuModel->Simulate(T,path,nbTimeSteps,scheduleSimulation);
    }
    else {
        simuModel->Simulate(t,T,path,past, nbTimeSteps,scheduleSimulation);
    }

    //For each asset, we shift the trajectory
    for (int d = 0; d < D; d++) {
        //Simulation of the path +h
        simuModel->ShiftAsset(pathShifted,path,d,h,t,timeStep);
        double payOffShifted = payOff(pathShifted,parameters,simuModel->rateModels);

        //Simulation of the path -h
        simuModel->ShiftAsset(pathShifted,path,d,moins_h,t,timeStep);
        payOffShifted -= payOff(pathShifted,parameters,simuModel->rateModels);

        LET(payOffDiff,d) = payOffShifted;
    }
}


MonteCarloPricer::~MonteCarloPricer() {
    pnl_mat_free(&path);
}
