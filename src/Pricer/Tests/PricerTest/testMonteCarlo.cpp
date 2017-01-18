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
    cout << "** Instance : ";
    Asset **assets = (Asset **)malloc(1 * sizeof(Asset*));
    assets[0] = new Asset("BNP","BNP",Change::EUR,0.03,spot,vol);
    PnlMat *choleskyCorr = pnl_mat_create_from_scalar(1,1,1.);
    assert(assets != NULL && assets[0] != NULL && choleskyCorr != NULL);
    RateModelGen **rateModels = (RateModelGen**) malloc(1 * sizeof(RateModelGen*));
    rateModels[0] = new ConstantRateModel(Change::EUR,FRR);
    assert(rateModels != NULL && rateModels[0] != NULL);
    ModelGen *modelBS = new BlackScholesModel(1,assets,choleskyCorr,1,rateModels);
    assert(modelBS != NULL);
    PricerGen *pricerMC = new MonteCarloPricer(maturity,modelBS,50000,(int)maturity);
    assert(pricerMC != NULL);
    ProductGen *call = new Call(pricerMC,assets,1,(int)maturity,strike);
    assert(call != NULL);
    Marche *marche = Marche::Instance(call);
    assert(marche != NULL);
    cout << " --> \033[1;34m [CHECK]\033[0m\n\n";

    call->PrintProduct();

    double price0, price1, ic;
    cout << "\n\n** Computing Price at t = 0 ... ";
    call->PriceProduct(0,price0,ic);
    price1 = pnl_bs_call(spot,strike,maturity,FRR,0.,vol);
    cout << " \033[1;34m [CHECK]\033[0m\n\n";
    cout << "--> Price : " << price0;
    cout << "\n--> Ic : " << ic;
    cout << "\n--> Closed formula Price : " << price1;
    assert(fabs(price0-price1) <= ic);

    // TODO : problème si t tombe sur un pas de constatation !
    double t = maturity / 2. + 0.2;
    cout << "\n\n** Computing Price at t = " << t << "\n";
    marche->ImportCotations(CotationTypes::Simulated);
    cout << "\nMarché (past) : \n\n";
    PnlMat subCoursMat = pnl_mat_wrap_mat_rows(marche->cours,0,(int)t);
    PnlMat *past = pnl_mat_copy(&subCoursMat);
    pnl_mat_print(past);
    spot = MGET(past,(int)t,0);
    cout << "Spot at t : " << spot;
    // TODO change with PriceProduct when marcheGetPast will be done
    call->pricer->Price(t,past,price0,ic,call->payOff,call->parameters);
    price1 = pnl_bs_call(spot,strike,maturity-t,FRR,0,vol);
    cout << "\n\n--> Price : " << price0;
    cout << "\n--> Ic : " << ic;
    cout << "\n--> Closed formula Price : " << price1;
    assert(fabs(price0-price1) <= ic);
    cout << " \n\033[1;34m [CHECK]\033[0m\n\n";

    cout << "\n\n** Delete : ";
    delete call;
    delete pricerMC;
    delete modelBS;
    delete rateModels[0];
    free(rateModels);
    pnl_mat_free(&choleskyCorr);
    delete assets[0];
    free(assets);
    cout << " --> \033[1;34m [CHECK]\033[0m\n\n";

    cout << "########################################\n\n";
    return EXIT_SUCCESS;
}
