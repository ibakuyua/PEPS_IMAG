#include <stdlib.h>
#include <iostream>
#include <assert.h>
#include "../../SimulationModels/BlackScholesModel.hpp"
#include "../../RateModels/ConstantRateModel.hpp"
#include "../../ProductInfo/infoMultimonde.hpp"
#include "../../FinancialProducts/Asset/ChangeZC.hpp"
#include "../../Stats/Parser/ParseCSV.h"
#include "../../FinancialProducts/Multimonde.hpp"

using namespace std;

int main(){
    cout << "\n\n###### TEST OF BLACKSCHOLES MODEL ######\n\n";
    cout << "** Instance : ";
    double maturity = 10.; // In term of days
    int stepNb = 10;
    RateModelGen **rateModels;
    rateModels = (RateModelGen **)malloc(6 * sizeof(RateModelGen*));
    assert(rateModels != NULL);
    for (int d = 0; d < 6; ++d) {
        rateModels[d] = new ConstantRateModel((Change)d,0.03/365);
    }
    ModelGen *blackScholes = new BlackScholesModel(11, 6, rateModels);
    assert(blackScholes != NULL);
    ParseCSV *parser = new ParseCSV("../data/dataPEPS.csv");
    StatsFactory *stats = new StatsFactory(parser->inputData);
    AssetList *assetList = Multimonde::GetAssetListFromStat(stats,blackScholes);
    blackScholes->SetAssets(assetList);
    PnlMat *path = pnl_mat_create(11,11);
    cout << " --> \033[1;34m [CHECK]\033[0m\n\n";
    cout << "** List of assets : \n";
    assetList->Print();
    cout << "\n** Simulation path : \n\n";
    blackScholes->Simulate(maturity,path,stepNb);
    pnl_mat_print(path);
    cout << "\n\n** Simulation market : \n\n";
    blackScholes->SimulateMarket(maturity,path,stepNb);
    pnl_mat_print(path);
    // Free
    cout << "\n\n** Delete : ";
    delete assetList;
    delete blackScholes;
    for (int d = 0; d < 6; ++d) {
        delete rateModels[d];
    }
    free(rateModels);
    cout << " --> \033[1;34m [CHECK]\033[0m\n\n";
    cout << "########################################\n\n";
    return EXIT_SUCCESS;
}
