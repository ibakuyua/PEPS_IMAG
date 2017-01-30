#include <stdlib.h>
#include <iostream>
#include <assert.h>
#include <pnl/pnl_finance.h>
#include "../../Pricing/PricerGen.hpp"
#include "../../Pricing/MonteCarloPricer.hpp"
#include "../../FinancialProducts/Call.hpp"
#include "../../SimulationModels/BlackScholesModel.hpp"
#include "../../RateModels/ConstantRateModel.hpp"
#include "../../Marche.hpp"

using namespace std;

int main(){

    cout << "\n\n###### TEST OF DELTA CALL ######\n\n";
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
    Call *call = new Call(pricer,asset,(int) 3,strike);

    assert(call != NULL);
    Marche *marche = Marche::Instance(Change::EUR,call);
    assert(marche != NULL);
    cout << " --> \033[1;34m [CHECK]\033[0m\n\n";
    call->Print();
    cout << "\n\n";

    double myPrice, myIc, price, deltaFF;

    //Computing delta
    PnlVect *delta = pnl_vect_create(1);
    PnlVect *icP = pnl_vect_create(1);
    PnlVect *parameters = pnl_vect_create_from_scalar(1,strike);
    cout << "\nMarché (past) : \n\n";
    marche->ImportCotations(CotationTypes::Simulated);
    PnlMat subCoursMat = pnl_mat_wrap_mat_rows(marche->cours,0,0);
    PnlMat *past = pnl_mat_copy(&subCoursMat);
    pnl_mat_print(past);
    cout << "\nComputing Delta at t = 0 ...\n";
    pnl_cf_call_bs(spot,strike,maturity,r,0.,volatility,&price,&deltaFF);
    pricer->Delta(0,past,delta,icP,payOffCall,parameters);
    pricer->Price(0,past,myPrice,myIc,payOffCall,parameters);
    cout << "Verification Prix : ";
    assert(fabs(price-myPrice) <= myIc);
    cout << " --> \033[1;34m [CHECK]\033[0m\n\n";

    cout << "Delta sur Call à 0 : \n";
    cout << "\n --> Closed Formula Delta : " << deltaFF;
    cout << "\n---> Delta computed : " << GET(delta,0);
    cout << "\n---> IC : [ " << GET(delta,0) - GET(icP,0)/2. << " ; " << GET(delta,0) + GET(icP,0)/2. << " ] --> largeur : " << GET(icP,0);

    // En t
    double t = 5.;
    subCoursMat = pnl_mat_wrap_mat_rows(marche->cours,0,2);
    past = pnl_mat_copy(&subCoursMat);
    cout << "\nMarché (past) : \n\n";
    pnl_mat_print(past);
    cout << "\n\n** Computing Delta at t = " << t << "\n";
    call->pricer->Delta(t,past,delta,icP,call->payOff,call->parameters);
    call->pricer->Price(t,past,myPrice,myIc,payOffCall,parameters);
    pnl_cf_call_bs(MGET(past,past->m-1,0),strike,maturity-t,r,0,volatility,&price,&deltaFF);
    cout << "Verification Prix : ";
    cout << " --> \033[1;34m [CHECK]\033[0m\n\n";
    cout << "\n --> Closed Formula Delta : " << deltaFF;
    cout << "\n---> Delta computed : " << GET(delta,0);
    cout << "\n---> IC : [ " << GET(delta,0) - GET(icP,0)/2. << " ; " << GET(delta,0) + GET(icP,0)/2. << " ]";
    cout << " \n\n\033[1;34m [CHECK]\033[0m";
    assert(fabs(price-myPrice) <= myIc);


    cout << "\n\nFree : ";
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

