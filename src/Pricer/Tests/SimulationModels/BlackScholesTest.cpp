#include <stdlib.h>
#include <iostream>
#include <assert.h>
#include "../../SimulationModels/BlackScholesModel.hpp"
#include "../../RateModels/ConstantRateModel.hpp"
#include "../../ProductInfo/infoMultimonde.hpp"
#include "../../FinancialProducts/Asset/ChangeZC.hpp"
#include "../../Stats/Parser/ParseCSV.h"

using namespace std;

Asset **SetParameter(PnlMat **correlMatrix, ModelGen *model, double maturity);
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
    ModelGen *blackScholes = new BlackScholesModel(11,6,rateModels);
    assert(blackScholes != NULL);
    PnlMat *correlMatrix = pnl_mat_create(11,11);
    Asset **myAssets = SetParameter(&correlMatrix, blackScholes, maturity);
    AssetList *assetList = new AssetList(11,myAssets,correlMatrix);
    blackScholes->SetAssets(assetList);
    PnlMat *path = pnl_mat_create(11,11);
    cout << " --> \033[1;34m [CHECK]\033[0m\n\n";
    cout << "** List of assets : \n";
    assetList->Print();
    cout << "\n** Simulation path : \n\n";
    blackScholes->Simulate(maturity,path,stepNb);
    pnl_mat_print(path);
    // Free
    cout << "\n\n** Delete : ";
    pnl_mat_free(&correlMatrix);
    for (int d = 0; d < 11; ++d) {
        delete myAssets[d];
    }
    free(myAssets);
    delete blackScholes;
    for (int d = 0; d < 6; ++d) {
        delete rateModels[d];
    }
    free(rateModels);
    cout << " --> \033[1;34m [CHECK]\033[0m\n\n";
    cout << "########################################\n\n";
    return EXIT_SUCCESS;
}

Asset **SetParameter(PnlMat **correlMatrix, ModelGen *model, double maturity){
    // Creation of the BlackScholes parameters in global economy
    // thanks to statistics in asset in their economy
    PnlVect *trend;
    PnlMat *volMatrixGlob;
    ParseCSV *parser = new ParseCSV("../data/dataPEPS.csv");
    StatsFactory *stats = new StatsFactory(parser->inputData);
    model->GetParametersFromStats(stats,&trend,&volMatrixGlob);
    // Creation of the BlackScoles parameters in euro economy
    PnlVect *trendEur = pnl_vect_copy(trend);
    PnlVect *sigma_i = pnl_vect_create(11);
    PnlVect *sigma_X_i = pnl_vect_create(11);
    LET(trendEur,0) = GET(trend,0);
    PnlMat *volMatrixEur = pnl_mat_copy(volMatrixGlob);
    for (int i = 1; i < 6; ++i) {
        pnl_mat_get_col(sigma_i,volMatrixGlob,i); // Get sigma_i
        pnl_mat_get_col(sigma_X_i,volMatrixGlob,i+5); // Get sigma_X_i
        // Xmu_i = mu_i + mu_X_i + sigma_i.sigma_X_i
        LET(trendEur,i) = GET(trend,i) + GET(trend,i+5)
                          + pnl_vect_scalar_prod(sigma_i,sigma_X_i);
        // Xsigma_i = sigma_i + sigma_X_i
        // TODO à tester si on a besoin de mettre la copie ?
        PnlVect *xsigma_i = pnl_vect_copy(sigma_i);
        pnl_vect_plus_vect(xsigma_i,sigma_X_i);
        pnl_mat_set_col(volMatrixEur,xsigma_i,i);
    }
    // compute volatility = sigma_i^2
    PnlVect *volEur = pnl_vect_create(11);
    for (int i = 0; i < 11; ++i) {
        pnl_mat_get_col(sigma_i,volMatrixEur,i);
        LET(volEur,i) = sqrt(pnl_vect_scalar_prod(sigma_i,sigma_i));
    }
    // Compute correlation matrix
    PnlMat *volMatrixEurT = pnl_mat_transpose(volMatrixEur);
    PnlMat *covMatrixEur = pnl_mat_mult_mat(volMatrixEur,volMatrixEurT);
    *correlMatrix = pnl_mat_create_from_scalar(11,11,1.);
    for (int i = 0; i < 11; ++i) {
        for (int j = 0; j < i; ++j) { // rho_ij = ( sigma.sigmaT_ij ) / ( sigma_i * sigma_j )
            double rho_ij = MGET(covMatrixEur,i,j) / ( GET(volEur,i) * GET(volEur,j) );
            MLET(*correlMatrix,i,j) = rho_ij;
            MLET(*correlMatrix,j,i) = rho_ij;
        }
    }
    // Creation of the assets
    Asset **myAssets = (Asset**) malloc(11 * sizeof(Asset*));
    myAssets[0] = new Asset(
            "EUR50","EUROSTOCK50",Change::EUR,GET(trendEur,0),SPOT_XEUROSTOCK50,GET(volEur,0));
    myAssets[1] = new Asset(
            "X_FTSE","X_FTSE",Change::EUR,GET(trendEur,1),SPOT_XFTSE,GET(volEur,1));
    myAssets[2] = new Asset(
            "X_P500","X_P500",Change::EUR,GET(trendEur,2),SPOT_XP500,GET(volEur,2));
    myAssets[3] = new Asset(
            "X_HANGSENG","X_HANGSENG",Change::EUR,GET(trendEur,3),SPOT_XHANGSENG,GET(volEur,3));
    myAssets[4] = new Asset(
            "X_NIKKEI","X_NIKKEI",Change::EUR,GET(trendEur,4),SPOT_XNIKKEI,GET(volEur,4));
    myAssets[5] = new Asset(
            "X_SPASX200","X_SPASX200",Change::EUR,GET(trendEur,5),SPOT_XSPASX200,GET(volEur,5));
    myAssets[6] = new ChangeZC(
            "EUR/GBP","EUR/GBP",Change::EUR,Change::GBP,GET(trendEur,6),
            SPOT_GBP * exp(model->rateModels[Change::GBP]->GetIntegralRate(0,maturity)),GET(volEur,6));
    myAssets[7] = new ChangeZC(
            "EUR/USD","EUR/USD",Change::EUR,Change::USD,GET(trendEur,7),
            SPOT_USD * exp(model->rateModels[Change::USD]->GetIntegralRate(0,maturity)),GET(volEur,7));
    myAssets[8] = new ChangeZC(
            "EUR/CNY","EUR/CNY",Change::EUR,Change::CNY,GET(trendEur,8),
            SPOT_CNY * exp(model->rateModels[Change::CNY]->GetIntegralRate(0,maturity)),GET(volEur,8));
    myAssets[9] = new ChangeZC(
            "EUR/JPY","EUR/JPY",Change::EUR,Change::JPY,GET(trendEur,9),
            SPOT_JPY * exp(model->rateModels[Change::JPY]->GetIntegralRate(0,maturity)),GET(volEur,9));
    myAssets[10] = new ChangeZC(
            "EUR/AUD","EUR/AUD",Change::EUR,Change::AUD,GET(trendEur,10),
            SPOT_AUD * exp(model->rateModels[Change::AUD]->GetIntegralRate(0,maturity)),GET(volEur,10));

    // Delete
    pnl_vect_free(&trend);
    pnl_vect_free(&trendEur);
    pnl_vect_free(&volEur);
    pnl_mat_free(&volMatrixGlob);
    pnl_vect_free(&sigma_i);
    pnl_vect_free(&sigma_X_i);
    pnl_mat_free(&volMatrixEur);
    pnl_mat_free(&volMatrixEurT);
    pnl_mat_free(&covMatrixEur);
    return myAssets;
}
