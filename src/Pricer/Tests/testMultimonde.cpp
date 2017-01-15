#include <stdlib.h>
#include <iostream>
#include <assert.h>
#include "../Pricing/PricerGen.hpp"
#include "../Pricing/MonteCarloPricer.hpp"
#include "../FinancialProducts/Multimonde.hpp"
#include "../SimulationModels/BlackScholesModel.hpp"
#include "../RateModels/ConstantRateModel.hpp"

using namespace std;

void setParameters(PnlMat **choleskyCorr, RateModelGen ***rateModels, Asset ***assets);
void freeParameters(PnlMat **choleskyCorr, RateModelGen ***rateModels, Asset ***assets);

int main(){

    cout << "\n\n###### TEST OF THE MULTIMONDE PRICER ######\n\n";
    cout << "** Instance : ";
    RateModelGen **rateModels;
    Asset **assets;
    PnlMat *choleskyCorr;
    setParameters(&choleskyCorr, &rateModels, &assets);
    ModelGen *simuIndex = new BlackScholesModel(6, assets,choleskyCorr,6,rateModels);
    int nbSample = 10000;
    int nbTimeStep = 2228;
    PricerGen * pricer = new MonteCarloPricer(
            Multimonde::maturity,simuIndex,nbSample,nbTimeStep);
    assert(pricer != NULL);
    Multimonde *multimonde = new Multimonde(pricer,assets,6);
    assert(multimonde != NULL);
    cout << " --> \033[1;34m [CHECK]\033[0m\n\n";
    double price, ic;
    cout << "Computing price ...\n";
    multimonde->PriceProduct(0, price, ic);
    cout << "\n--> Price : " << price;
    cout << "\n--> Ic : " << ic;
    // Free
    cout << "\n\n** Delete : ";
    delete multimonde;
    delete pricer;
    freeParameters(&choleskyCorr,&rateModels,&assets);
    cout << " --> \033[1;34m [CHECK]\033[0m\n\n";

    cout << "########################################\n\n";
    return EXIT_SUCCESS;
}

void setParameters(PnlMat **choleskyCorr, RateModelGen ***rateModels, Asset ***assets){
    // TODO : Peut être faire des méthodes venant de multimonde
    *rateModels = (RateModelGen**) malloc(6* sizeof(RateModelGen*));
    for (int d = 0; d < 6; ++d)
        (*rateModels)[d] = new ConstantRateModel((Change)d, 0.03);
    *assets = (Asset**) malloc(6 * sizeof(Asset*));
    (*assets)[0] = new Asset(string("FTSE"), string("FTSE"), Change::GBP, TREND_FTSE, SPOT_FTSE, VOL_FTSE);
    (*assets)[1] = new Asset(string("P500"), string("P500"), Change::USD, TREND_P500, SPOT_P500, VOL_P500);
    (*assets)[2] = new Asset(string("HANGSENG"), string("HANGSENG"), Change::HKD, TREND_HANGSENG, SPOT_HANGSENG, VOL_HANGSENG);
    (*assets)[3] = new Asset(string("NIKKEI"), string("NIKKEI"), Change::JPY , TREND_NIKKEI, SPOT_NIKKEI, VOL_NIKKEI);
    (*assets)[4] = new Asset(string("SPASX200"), string("SPASX200"), Change::AUD, TREND_SPASX200, SPOT_SPASX200, VOL_SPASX200);
    (*assets)[5] = new Asset(string("EUROSTOCK50"), string("EUROSTOCK50"), Change::EUR, TREND_EUROSTOCK50, SPOT_EUROSTOCK50, VOL_EUROSTOCK50);
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
void freeParameters(PnlMat **choleskyCorr, RateModelGen ***rateModels,Asset ***assets){
    pnl_mat_free(choleskyCorr);
    for (int i = 0; i < 6; ++i) {
        delete (*rateModels)[i];
    }
    delete *rateModels;
    for (int i = 0; i < 6; ++i) {
        delete (*assets)[i];
    }
    delete *assets;
}
