#include <stdlib.h>
#include <iostream>
#include <assert.h>
#include <pnl/pnl_finance.h>
#include "../Pricing/PricerGen.hpp"
#include "../Pricing/MonteCarloPricer.hpp"
#include "../FinancialProducts/Call.hpp"
#include "../SimulationModels/BlackScholesModel.hpp"
#include "../RateModels/ConstantRateModel.hpp"
#include "../Marche.hpp"

using namespace std;

#define NB_TIMEVALUE_KNOWN 1 // Must be equal to (int)(t*N/maturity)+2]
#define NB_ASSET 1 // Must be equal to size
#define NB_ECONOMY 1

int main(){

    cout << "\n\n###### TEST OF HEDGING CALL ######\n\n";
    cout << "** Instance : ";

    //Useful datas
    double strike = 100.;
    double trend = 0.03;
    double spot = 100.;
    double volatility = 0.15;
    double r = 0.03;
    double maturity = 10.;
    int nbSample = 200000;


    Asset *asset = new Asset("XX","MyAsset",Change::EUR,trend,spot,volatility);
    assert(asset != NULL);

    RateModelGen **rateModels = (RateModelGen**) malloc(1 * sizeof(RateModelGen*));
    rateModels[0] = new ConstantRateModel(Change::EUR,r);
    assert(rateModels != NULL && rateModels[0] != NULL);

    //Initialisation du Modele de BlackScholes
    ModelGen *modelBlackScholes = new BlackScholesModel(1, 1, rateModels);
    assert(modelBlackScholes != NULL);
    //Initialisation du Pricer MonteCarlo
    PnlVect *schedule = pnl_vect_create(2);
    LET(schedule,0) = 4.;
    LET(schedule,1) = 6.;
    PricerGen *pricer = new MonteCarloPricer(maturity, modelBlackScholes, schedule, nbSample);
    assert(pricer != NULL);
    //Initialisation of Call Product
    Call *call = new Call(pricer,asset,(int) maturity,strike);
    assert(call != NULL);
    cout << " --> \033[1;34m [CHECK]\033[0m\n\n";
    call->Print();
    cout << "\n\n";
    Marche *marche = Marche::Instance(call);
    assert(marche != NULL);
    cout << " --> \033[1;34m [CHECK]\033[0m\n\n";

    double price0, price1, ic, deltaFF;
    cout << "\n\n** Computing Price at t = 0 ... ";
    call->PriceProduct(0,price0,ic);
    pnl_cf_call_bs(spot,strike,maturity,r,0.,volatility,&price1,&deltaFF);
    cout << " \033[1;34m [CHECK]\033[0m\n\n";
    cout << "--> Price : " << price0;
    cout << "\n--> Ic : [ " << price0 - ic/2 << " ; " << price0 + ic/2 << " ] --> width : " << ic;
    cout << "\n--> Closed formula Price : " << price1;
    assert(fabs(price0-price1) <= ic);


    //Spot Vector for Past Matrix
    //cout << "\n\n=== Initialisation of Spot Vector ===\n";
    PnlVect *spotV = pnl_vect_create(NB_ASSET);
    for (int d = 0; d < NB_ASSET; ++d) {
        PNL_SET(spotV, d, spot);
    }
    cout << "\n\nCHECK : \n\n";


    //Setting past
    PnlMat *past = pnl_mat_create_from_zero(1,NB_ASSET);
    pnl_mat_set_row(past,spotV,0);

    cout << "PAST :\n";
    pnl_mat_print(past);

    //Computing delta
    PnlVect *delta = pnl_vect_create(1);
    PnlVect *parameters = pnl_vect_create_from_scalar(1,strike);
    cout << "\nComputing Delta ...\n";
    pricer->Delta(0,past,delta,payOffCall,parameters);

    cout << "Delta sur Call à 0 : \n";
    cout << "\n --> Closed Formula Delta : " << deltaFF;
    cout << "\n---> Delta computed : \n";
    pnl_vect_print(delta);

    // En t
    double t = 5.3;
    cout << "\n\n** Computing Price at t = " << t << "\n";
    marche->ImportCotations(CotationTypes::Simulated);
    cout << "\nMarché (past) : \n\n";
    PnlMat subCoursMat = pnl_mat_wrap_mat_rows(marche->cours,0,2);
    past = pnl_mat_copy(&subCoursMat);
    pnl_mat_print(past);
    spot = MGET(past,past->m-1,0);
    cout << "Spot at t : " << spot;
    call->pricer->Price(t,past,price0,ic,call->payOff,call->parameters);
    call->pricer->Delta(t,past,delta,call->payOff,call->parameters);
    pnl_cf_call_bs(spot,strike,maturity-t,r,0,volatility,&price1,&deltaFF);
    cout << "\n\n--> Price : " << price0;
    cout << "\n--> Ic : [ " << price0 - ic/2 << " ; " << price0 + ic/2 << " ] --> width : " << ic ;
    cout << "\n--> Closed formula Price : " << price1;
    assert(fabs(price0-price1) <= ic);
    cout << "Delta sur Call à t : \n";
    cout << "\n --> Closed Formula Delta : " << deltaFF;
    cout << "\n---> Delta computed : \n";
    pnl_vect_print(delta);
    cout << " \n\n\033[1;34m [CHECK]\033[0m";


    //FREEING Memory
    delete call;
    delete pricer;
    delete modelBlackScholes;
    delete rateModels[0];
    free(rateModels);
    delete asset;
    cout << " --> \033[1;34m [CHECK]\033[0m\n\n";

    cout << "########################################\n\n";
    return EXIT_SUCCESS;

}

