#include "Multimonde.hpp"
#include "Asset/ChangeZC.hpp"


Multimonde::Multimonde(PricerGen *pricer, int hedgingDateNb, StatsFactory *stats)
    : ProductGen("Multimonde21", pricer, hedgingDateNb, payOffMultimonde21)
{
    // Creation of the BlackScholes parameters in global economy
    // thanks to statistics in asset in their economy
    PnlVect *trend;
    PnlMat *volMatrix;
    pricer->simuModel->GetParametersFromStats(stats,&trend,&volMatrix);
    // Creation of the BlackScoles parameters in euro economy
    PnlVect *trendEur = pnl_vect_copy(trend);
    PnlVect *sigma_i = pnl_vect_create(11);
    PnlVect *sigma_X_i = pnl_vect_create(11);
    LET(trendEur,0) = GET(trend,0);
    PnlMat *volMatrixEur = pnl_mat_copy(volMatrix);
    for (int i = 1; i < 6; ++i) {
        pnl_mat_get_col(sigma_i,volMatrix,i); // Get sigma_i
        pnl_mat_get_col(sigma_X_i,volMatrix,i+5); // Get sigma_X_i
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
            "EUR/GBP","EUR/GBP",Change::EUR,Change::GBP,GET(trendEur,6),SPOT_GBP,GET(volEur,6));
    myAssets[7] = new ChangeZC(
            "EUR/USD","EUR/USD",Change::EUR,Change::USD,GET(trendEur,7),SPOT_USD,GET(volEur,7));
    myAssets[8] = new ChangeZC(
            "EUR/CNY","EUR/CNY",Change::EUR,Change::CNY,GET(trendEur,8),SPOT_CNY,GET(volEur,8));
    myAssets[9] = new ChangeZC(
            "EUR/JPY","EUR/JPY",Change::EUR,Change::JPY,GET(trendEur,9),SPOT_JPY,GET(volEur,9));
    myAssets[10] = new ChangeZC(
            "EUR/AUD","EUR/AUD",Change::EUR,Change::AUD,GET(trendEur,10),SPOT_AUD,GET(volEur,10));
    assets = new AssetList(11,myAssets,volMatrixEur,true);
    pricer->simuModel->SetAssets(assets);

    // Delete
    pnl_vect_free(&trend);
    pnl_vect_free(&trendEur);
   pnl_vect_free(&volEur);
    pnl_mat_free(&volMatrix);
    pnl_vect_free(&sigma_i);
    pnl_vect_free(&sigma_X_i);
    //pnl_mat_free(&volMatrixEur); deleted in the destructor of assets
}

Multimonde::~Multimonde() {
    for (int i = 0; i < 11; ++i) {
        delete assets->assets[i];
    }
    delete assets;
}


void Multimonde::MAJPortfolio() {
    // TODO with market
}

