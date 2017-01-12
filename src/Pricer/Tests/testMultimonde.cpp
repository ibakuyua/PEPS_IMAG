#include <stdlib.h>
#include <iostream>
#include <assert.h>
#include "../Pricing/PricerGen.hpp"
#include "../Pricing/MonteCarloPricer.hpp"
#include "../Multimonde.hpp"
#include "../SimulationModels/SimuIndex/BlackScholesIndexModel.hpp"
#include "../RateModels/ConstantRateModel.hpp"

using namespace std;

void setParameters(PnlVect **spot, PnlVect **trend, PnlVect **volatility, PnlMat **choleskyCorr, RateModelGen ***rateModels);
void freeParameters(PnlVect **spot, PnlVect **trend, PnlVect **volatility, PnlMat **choleskyCorr, RateModelGen ***rateModels);

int main(){

    cout << "\n\n###### TEST OF THE MULTIMONDE PRICER ######\n\n";
    cout << "** Instance : ";
    RateModelGen **rateModels;
    PnlVect *spot, *trend, *volatility;
    PnlMat *choleskyCorr;
    setParameters(&spot,&trend,&volatility,&choleskyCorr, &rateModels);
    ModelGen *simuIndex = new BlackScholesIndexModel(6,spot, trend, volatility, choleskyCorr, rateModels);
    ModelGen *simuChange;
    int nbSample = 50000;
    int nbTimeStep = 100;
    PricerGen * pricer = new MonteCarloPricer(
            Multimonde::maturity,Multimonde::payOff,simuIndex,simuChange,nbSample, nbTimeStep);
    assert(pricer != NULL);
    Multimonde *multimonde = new Multimonde(pricer);
    assert(multimonde != NULL);
    cout << " --> \033[1;34m [CHECK]\033[0m\n\n";
    double price, ic;
    cout << "Computing price ...\n";
    multimonde->PriceMultimonde(0, price, ic);
    cout << "\n--> Price : " << price;
    cout << "\n--> Ic : " << ic;
    // Free
    cout << "\n\n** Delete : ";
    delete multimonde;
    delete pricer;
    freeParameters(&spot,&trend,&volatility,&choleskyCorr,&rateModels);
    cout << " --> \033[1;34m [CHECK]\033[0m\n\n";

    cout << "########################################\n\n";
    return EXIT_SUCCESS;
}

void setParameters(PnlVect **spot, PnlVect **trend, PnlVect **volatility, PnlMat **choleskyCorr, RateModelGen ***rateModels){
    // TODO : Peut être faire des méthodes venant de multimonde
    *rateModels = (RateModelGen**) malloc(6* sizeof(RateModelGen*));
    for (int d = 0; d < 6; ++d)
        (*rateModels)[d] = new ConstantRateModel(0.03);
    *spot = pnl_vect_create(6);
    PNL_SET(*spot,0,SPOT_FTSE);
    PNL_SET(*spot,1,SPOT_P500);
    PNL_SET(*spot,2,SPOT_HANGSENG);
    PNL_SET(*spot,3,SPOT_NIKKEI);
    PNL_SET(*spot,4,SPOT_SPASX200);
    PNL_SET(*spot,5,SPOT_EUROSTOCK50);
    *trend = pnl_vect_create_from_scalar(6,0.07);
    *volatility = pnl_vect_create(6);
    PNL_SET(*volatility,0,VOL_FTSE);
    PNL_SET(*volatility,1,VOL_P500);
    PNL_SET(*volatility,2,VOL_HANGSENG);
    PNL_SET(*volatility,3,VOL_NIKKEI);
    PNL_SET(*volatility,4,VOL_SPASX200);
    PNL_SET(*volatility,5,VOL_EUROSTOCK50);
    *choleskyCorr = pnl_mat_create(6,6);
    PNL_MSET(*choleskyCorr,0,1,COR_FTSE_P500);
    PNL_MSET(*choleskyCorr,0,2,COR_FTSE_HANGSENG);
    PNL_MSET(*choleskyCorr,0,3,COR_FTSE_NIKKEI);
    PNL_MSET(*choleskyCorr,0,4,COR_FTSE_SPASX200);
    PNL_MSET(*choleskyCorr,0,5,COR_FTSE_EUROSTOCK50);
    PNL_MSET(*choleskyCorr,1,2,COR_P500_HANGSENG);
    PNL_MSET(*choleskyCorr,1,3,COR_P500_NIKKEI);
    PNL_MSET(*choleskyCorr,1,4,COR_P500_SPASX200);
    PNL_MSET(*choleskyCorr,1,5,COR_P500_EUROSTOCK50);
    PNL_MSET(*choleskyCorr,2,3,COR_HANGSENG_NIKKEI);
    PNL_MSET(*choleskyCorr,2,4,COR_HANGSENG_SPASX200);
    PNL_MSET(*choleskyCorr,2,5,COR_HANGSENG_EUROSTOCK50);
    PNL_MSET(*choleskyCorr,3,4,COR_NIKKEI_SPASX200);
    PNL_MSET(*choleskyCorr,3,5,COR_NIKKEI_EUROSTOCK50);
    PNL_MSET(*choleskyCorr,4,5,COR_SPASX_EUROSTOCK50);
    for (int i = 0; i < 6; ++i)
        for (int j = i; j < 6; ++j) {
            if (i==j)
                PNL_MSET(*choleskyCorr, i, j, 1);
            else
                PNL_MSET(*choleskyCorr, j, i, PNL_MGET(*choleskyCorr, i, j));
        }
    pnl_mat_chol(*choleskyCorr);
}
void freeParameters(PnlVect **spot, PnlVect **trend, PnlVect **volatility, PnlMat **choleskyCorr, RateModelGen ***rateModels){
    pnl_vect_free(spot);
    pnl_vect_free(trend);
    pnl_vect_free(volatility);
    pnl_mat_free(choleskyCorr);
    for (int i = 0; i < 6; ++i) {
        delete (*rateModels)[i];
    }
    delete *rateModels;
}
