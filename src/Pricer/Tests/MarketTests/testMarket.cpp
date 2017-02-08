#include <iostream>
#include <assert.h>
#include "../../FinancialProducts/Asset/Asset.hpp"
#include "../../Marche.hpp"
#include "../../RateModels/ConstantRateModel.hpp"
#include "../../SimulationModels/BlackScholesModel.hpp"
#include "../../FinancialProducts/Call.hpp"
#include "../../Pricing/MonteCarloPricer.hpp"

using namespace std;

/////// TEST MARKET ////////
int main(){
    double spot = 100.;
    double vol = 0.15;
    double FRR = 0.03;
    double maturity = 10.;
    double strike = 100.;
    int simuNb = (int)(maturity/2);
    int sampleNb = 200000;
    cout << "** Instance : ";
    Asset * asset = new Asset("BNP","BNP",Change::EUR,0.03,spot,vol);
    assert(asset != NULL);
    RateModelGen **rateModels = (RateModelGen**) malloc(1 * sizeof(RateModelGen*));
    rateModels[0] = new ConstantRateModel(Change::EUR,FRR);
    assert(rateModels != NULL && rateModels[0] != NULL);
    ModelGen *modelBS = new BlackScholesModel(1, 1, rateModels);
    assert(modelBS != NULL);
    PricerGen *pricerMC = new MonteCarloPricer(maturity, modelBS, simuNb, sampleNb);
    assert(pricerMC != NULL);
    ProductGen *call = new Call(pricerMC,asset,(int)maturity,strike);
    assert(call != NULL);
    Marche *marche = Marche::Instance(Change::EUR,call);
    assert(marche != NULL);
    cout << " --> \033[1;34m [CHECK]\033[0m\n\n";

    marche->ImportCotations(CotationTypes::Simulated);
    cout << " \n\nSimulated market : \n\n";
    pnl_mat_print(marche->cours);
    cout << "--> \033[1;34m [CHECK]\033[0m\n\n";

    cout << "Get Quotations at t = 0. \n\n";
    PnlVect * spotV = pnl_vect_new();
    marche->GetCotations(0,spotV);
    pnl_vect_print(spotV);
    assert(GET(spotV,0) == MGET(marche->cours,0,0));
    cout << "--> \033[1;34m [CHECK]\033[0m\n\n";
    cout << "Get Quotations at t = 0.5 \n\n";
    marche->GetCotations(0.5,spotV);
    pnl_vect_print(spotV);
    assert(GET(spotV,0) == MGET(marche->cours,0,0));
    cout << "--> \033[1;34m [CHECK]\033[0m\n\n";
    cout << "Get Quotations at t = 2.7 \n\n";
    marche->GetCotations(2.7,spotV);
    pnl_vect_print(spotV);
    assert(GET(spotV,0) == MGET(marche->cours,2,0));
    cout << "--> \033[1;34m [CHECK]\033[0m\n\n";
    cout << "Get Quotations at t = 3. \n\n";
    marche->GetCotations(3.,spotV);
    pnl_vect_print(spotV);
    assert(GET(spotV,0) == MGET(marche->cours,3,0));
    cout << "--> \033[1;34m [CHECK]\033[0m\n\n";
    cout << "Get Quotations at t = 10 \n\n";
    marche->GetCotations(10.,spotV);
    pnl_vect_print(spotV);
    assert(GET(spotV,0) == MGET(marche->cours,10,0));
    cout << "--> \033[1;34m [CHECK]\033[0m\n\n";

    cout << "Get past quotations (hedging indexes) t = 0.\n\n";
    PnlMat *past = pnl_mat_new();
    marche->GetPastCotations(0.,past);
    pnl_mat_print(past);
    assert(past->m == 1);
    cout << "--> \033[1;34m [CHECK]\033[0m\n\n";
    cout << "Get past quotations (hedging indexes) t = 0.5\n\n";
    marche->GetPastCotations(0.3,past);
    pnl_mat_print(past);
    assert(past->m == 1);
    cout << "--> \033[1;34m [CHECK]\033[0m\n\n";
    cout << "Get past quotations (hedging indexes) t = 1.\n\n";
    marche->GetPastCotations(1.,past);
    pnl_mat_print(past);
    assert(past->m == 2);
    cout << "--> \033[1;34m [CHECK]\033[0m\n\n";
    cout << "Get past quotations (hedging indexes) t = 10.\n\n";
    marche->GetPastCotations(10.,past);
    pnl_mat_print(past);
    cout << "--> \033[1;34m [CHECK]\033[0m\n\n";
    assert(past->m == 11);

    cout << "Get past quotations (sample indexes 1/2) t = 0\n\n";
    marche->GetPastCotations(0,past,true,simuNb);
    pnl_mat_print(past);
    assert(past->m == 1);
    cout << "--> \033[1;34m [CHECK]\033[0m\n\n";
    cout << "Get past quotations (sample indexes 1/2) t = 0.5\n\n";
    marche->GetPastCotations(0.5,past,true,simuNb);
    pnl_mat_print(past);
    assert(past->m == 1);
    cout << "--> \033[1;34m [CHECK]\033[0m\n\n";
    cout << "Get past quotations (sample indexes 1/2) t = 1.\n\n";
    marche->GetPastCotations(1.,past,true,simuNb);
    pnl_mat_print(past);
    assert(past->m == 2 && MGET(past,past->m-1,0) == MGET(marche->cours,1,0));
    cout << "--> \033[1;34m [CHECK]\033[0m\n\n";
    cout << "Get past quotations (sample indexes 1/2) t = 2.5\n\n";
    marche->GetPastCotations(2.5,past,true,simuNb);
    pnl_mat_print(past);
    assert(past->m == 2 && MGET(past,past->m-1,0) == MGET(marche->cours,2,0));
    cout << "--> \033[1;34m [CHECK]\033[0m\n\n";
    cout << "Get past quotations (sample indexes 1/2) t = 3.7\n\n";
    marche->GetPastCotations(3.7,past,true,simuNb);
    pnl_mat_print(past);
    assert(past->m == 3 && MGET(past,past->m-1,0) == MGET(marche->cours,3,0) && MGET(past,past->m-2,0) == MGET(marche->cours,2,0));
    cout << "--> \033[1;34m [CHECK]\033[0m\n\n";
    cout << "Get past quotations (sample indexes 1/2) t = 10.\n\n";
    marche->GetPastCotations(10.,past,true,simuNb);
    pnl_mat_print(past);
    assert(past->m == 6);
    cout << "--> \033[1;34m [CHECK]\033[0m\n\n";

    // Free
    cout << "\n\n** Delete : ";
    pnl_mat_free(&past);
    pnl_vect_free(&spotV);
    delete call;
    delete pricerMC;
    delete modelBS;
    delete rateModels[0];
    free(rateModels);
    delete asset;
    cout << " --> \033[1;34m [CHECK]\033[0m\n\n";

    return EXIT_SUCCESS;
}
