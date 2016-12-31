#include <stdlib.h>
#include <iostream>
#include <assert.h>
#include "../Pricing/PricerGen.hpp"
#include "../Pricing/MonteCarloPricer.hpp"
#include "../Multimonde.hpp"
#include "../SimulationModels/SimuIndex/BlackScholesIndexModel.hpp"
#include "../RateModels/ConstantRateModel.hpp"

using namespace std;

void setParameters(PnlVect **spot, PnlVect **trend, PnlVect **volatility, PnlMat **choleskyCorr);

int main(){

    cout << "\n\n###### TEST OF THE MULTIMONDE PRICER ######\n\n";
    cout << "** Instance : ";
    double frr = 0.03;
    RateModelGen *constRate = new ConstantRateModel(frr);
    PnlVect *spot, *trend, *volatility;
    PnlMat *choleskyCorr;
    setParameters(&spot,&trend,&volatility,&choleskyCorr);
    ModelGen *simuIndex = new BlackScholesIndexModel(6,spot, trend, volatility, choleskyCorr, constRate);
    ModelGen *simuChange;
    int nbSample = 50000;
    int nbTimeStep = 9000;
    PricerGen * pricer = new MonteCarloPricer(
            Multimonde::maturity,Multimonde::payOff,simuIndex,simuChange,nbSample, nbTimeStep);
    assert(pricer != NULL);
    Multimonde *multimonde = new Multimonde(pricer);
    assert(multimonde != NULL);
    cout << " --> \033[1;34m [CHECK]\033[0m\n\n";
    double price, ic;
    multimonde->PriceMultimonde(0, price, ic);
    cout << "\n\n--> Price : " << price;
    cout << "\n--> Ic : " << ic;
    // Free
    cout << "\n\n** Delete : ";
    delete multimonde;
    delete pricer;
    pnl_vect_free(&spot);
    pnl_vect_free(&trend);
    pnl_vect_free(&volatility);
    pnl_mat_free(&choleskyCorr);
    cout << " --> \033[1;34m [CHECK]\033[0m\n\n";

    cout << "########################################\n\n";
    return EXIT_SUCCESS;
}

void setParameters(PnlVect **spot, PnlVect **trend, PnlVect **volatility, PnlMat **choleskyCorr){
    *spot = pnl_vect_create_from_scalar(6,100);
    *trend = pnl_vect_create_from_scalar(6,0.07);
    *volatility = pnl_vect_create_from_scalar(6,0.04);
    *choleskyCorr = pnl_mat_create_from_scalar(6,6,0.01);
    for (int i = 0; i < 6; ++i)
        PNL_MSET(*choleskyCorr,i,i,1);
}
