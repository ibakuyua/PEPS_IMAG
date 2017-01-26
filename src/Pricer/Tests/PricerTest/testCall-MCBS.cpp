#include <iostream>
#include <assert.h>
#include "../../FinancialProducts/ProductGen.hpp"
#include "../../FinancialProducts/Call.hpp"
#include "../../Pricing/MonteCarloPricer.hpp"
#include "../../SimulationModels/BlackScholesModel.hpp"
#include "../../RateModels/ConstantRateModel.hpp"
#include "pnl/pnl_finance.h"
#include "../../Marche.hpp"

using namespace std;

int main(){
    cout << "\n\n##### TEST OF MONTECARLO PRICER #####\n\n";
    double spot = 100.;
    double vol = 0.15;
    double FRR = 0.03;
    double maturity = 10.;
    double strike = 100.;
    int sampleNb = 200000;
    cout << "** Instance : ";
    Asset * asset = new Asset("BNP","BNP",Change::EUR,0.03,spot,vol);
    assert(asset != NULL);
    RateModelGen **rateModels = (RateModelGen**) malloc(1 * sizeof(RateModelGen*));
    rateModels[0] = new ConstantRateModel(Change::EUR,FRR);
    assert(rateModels != NULL && rateModels[0] != NULL);
    ModelGen *modelBS = new BlackScholesModel(1,1,rateModels);
    assert(modelBS != NULL);
    PricerGen *pricerMC = new MonteCarloPricer(maturity,modelBS,sampleNb,(int)maturity);
    assert(pricerMC != NULL);
    ProductGen *call = new Call(pricerMC,asset,(int)maturity,strike);
    assert(call != NULL);
    Marche *marche = Marche::Instance(call);
    assert(marche != NULL);
    cout << " --> \033[1;34m [CHECK]\033[0m\n\n";

    call->Print();

    double price0, price1, ic;
    cout << "\n\n** Computing Price at t = 0 ... ";
    call->PriceProduct(0,price0,ic);
    price1 = pnl_bs_call(spot,strike,maturity,FRR,0.,vol);
    cout << " \033[1;34m [CHECK]\033[0m\n\n";
    cout << "--> Price : " << price0;
    cout << "\n--> Ic : [ " << price0 - ic/2 << " ; " << price0 + ic/2 << " ] --> width : " << ic;
    cout << "\n--> Closed formula Price : " << price1;
    assert(fabs(price0-price1) <= ic);

    // TODO : problème si t tombe sur un pas de constatation ! past a un élément de plus
    double t = maturity / 2. + 0.3;
    cout << "\n\n** Computing Price at t = " << t << "\n";
    marche->ImportCotations(CotationTypes::Simulated);
    cout << "\nMarché (past) : \n\n";
    PnlMat subCoursMat = pnl_mat_wrap_mat_rows(marche->cours,0,(int)t+1);
    PnlMat *past = pnl_mat_copy(&subCoursMat);
    pnl_mat_print(past);
    spot = MGET(past,past->m-1,0);
    cout << "Spot at t : " << spot;
    // TODO change with PriceProduct when marcheGetPast will be done
    call->pricer->Price(t,past,price0,ic,call->payOff,call->parameters);
    price1 = pnl_bs_call(spot,strike,maturity-t,FRR,0,vol);
    cout << "\n\n--> Price : " << price0;
    cout << "\n--> Ic : [ " << price0 - ic/2 << " ; " << price0 + ic/2 << " ] --> width : " << ic ;
    cout << "\n--> Closed formula Price : " << price1;
    assert(fabs(price0-price1) <= ic);
    cout << " \n\n\033[1;34m [CHECK]\033[0m";

    cout << "\n\n** Delete : ";
    delete call;
    delete pricerMC;
    delete modelBS;
    delete rateModels[0];
    free(rateModels);
    delete asset;
    cout << " --> \033[1;34m [CHECK]\033[0m\n\n";

    cout << "########################################\n\n";
    return EXIT_SUCCESS;
}
