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

    //TODO : faire les const dans toutes les fonctions là où ça va bien

    cout << "\n\n###### TEST OF THE MULTIMONDE PRICER ######\n\n";
    cout << "** Instance : ";
    RateModelGen **rateModels;
    Asset **assets;
    PnlMat *choleskyCorr;
    setParameters(&choleskyCorr, &rateModels, &assets);
    ModelGen *simuIndex = new BlackScholesModel(11, 6,rateModels);
    int nbSample = 5000;
    int hedgingNb, nbTimeStep;
    hedgingNb = nbTimeStep = (int)Multimonde::maturity; // TODO vérifier qu'on fait bien jour à jour (constructeur multimonde ?)
    PricerGen * pricer = new MonteCarloPricer(
            Multimonde::maturity,simuIndex,nbSample,nbTimeStep);
    assert(pricer != NULL);
    AssetList *assetList = new AssetList(11,assets,choleskyCorr);
    Multimonde *multimonde = new Multimonde(pricer,hedgingNb,assetList);
    assert(multimonde != NULL);
    cout << " --> \033[1;34m [CHECK]\033[0m\n\n";
    multimonde->Print();
    cout << "\n\n";
    double price, ic;
    /*cout << "Computing Price ...\n";
    multimonde->PriceProduct(0, price, ic);
    cout << "\n--> Price : " << price;
    cout << "\n--> Ic : " << ic;*/
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
    *rateModels = (RateModelGen**) malloc(1* sizeof(RateModelGen*));
    for (int d = 0; d < 6; ++d)
        (*rateModels)[d] = new ConstantRateModel((Change)d, 0.03);
    *assets = (Asset**) malloc(11 * sizeof(Asset*));
    (*assets)[0] = new Asset(string("EUROSTOCK50"), string("EUROSTOCK50"), Change::EUR, TREND_EUROSTOCK50, SPOT_EUROSTOCK50, VOL_EUROSTOCK50);
    (*assets)[1] = new Asset(string("X_FTSE"), string("X_FTSE"), Change::EUR, TREND_FTSE, SPOT_FTSE, VOL_FTSE);
    (*assets)[2] = new Asset(string("X_P500"), string("X_P500"), Change::EUR, TREND_P500, SPOT_P500, VOL_P500);
    (*assets)[3] = new Asset(string("X_HANGSENG"), string("X_HANGSENG"), Change::EUR, TREND_HANGSENG, SPOT_HANGSENG, VOL_HANGSENG);
    (*assets)[4] = new Asset(string("X_NIKKEI"), string("X_NIKKEI"), Change::EUR, TREND_NIKKEI, SPOT_NIKKEI, VOL_NIKKEI);
    (*assets)[5] = new Asset(string("X_SPASX200"), string("X_SPASX200"), Change::EUR, TREND_SPASX200, SPOT_SPASX200, VOL_SPASX200);
    (*assets)[6] = new Asset(string("X_Rgbp"), string("X_Rgbp"), Change::EUR, TREND_SPASX200, SPOT_SPASX200, VOL_SPASX200);
    (*assets)[7] = new Asset(string("X_Rusd"), string("X_Rusd"), Change::EUR, TREND_SPASX200, SPOT_SPASX200, VOL_SPASX200);
    (*assets)[8] = new Asset(string("X_Rhkd"), string("X_Rhkd"), Change::EUR, TREND_SPASX200, SPOT_SPASX200, VOL_SPASX200);
    (*assets)[9] = new Asset(string("X_Rjpy"), string("X_Rjpy"), Change::EUR, TREND_SPASX200, SPOT_SPASX200, VOL_SPASX200);
    (*assets)[10] = new Asset(string("X_Raud"), string("X_Raud"), Change::EUR, TREND_SPASX200, SPOT_SPASX200, VOL_SPASX200);
    *choleskyCorr = pnl_mat_create_from_scalar(11,11,0.);
    // TODO : changer l'ordre
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
    for (int i = 0; i < 11; ++i)
        for (int j = i; j < 11; ++j) {
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
    for (int i = 0; i < 11; ++i) {
        delete (*assets)[i];
    }
    delete *assets;
}
