#include <stdlib.h>
#include <iostream>
#include <assert.h>
#include <pnl/pnl_finance.h>
#include "../Pricing/PricerGen.hpp"
#include "../Pricing/MonteCarloPricer.hpp"
#include "../FinancialProducts/Call.hpp"
#include "../SimulationModels/BlackScholesModel.hpp"
#include "../RateModels/ConstantRateModel.hpp"
#include "../Stats/Parser/ParseCSV.h"
#include "../FinancialProducts/ProductGen.hpp"

using namespace std;

#define NB_TIMEVALUE_KNOWN 1 // Must be equal to (int)(t*N/maturity)+2]
#define NB_ASSET 1 // Must be equal to size
#define NB_ECONOMY 1

int main(){

    cout << "\n\n###### TEST OF HEDGING CALL ######\n\n";
    cout << "** Instance : ";

    //Useful datas
    double strike = 100.;
    double trend = 0.02;
    double spot = 100.;
    double volatility = 0.2;
    double r = 0.02;
    double maturity = 10.;

    //Initialisation du modele de taux
    RateModelGen **rateModels = (RateModelGen**) malloc(1 * sizeof(RateModelGen*));
    rateModels[0] = new ConstantRateModel(Change::EUR,r);
    assert(rateModels != NULL && rateModels[0] != NULL);

    //Initialisation du Modele de BlackScholes
    ModelGen *simuIndex = new BlackScholesModel(NB_ASSET, NB_ECONOMY,rateModels);

    //Spot Vector for simu BS
    cout << "=== Initialisation of Spot Vector ===\n";
    PnlVect *spotV = pnl_vect_create(NB_ASSET);
    for (int d = 0; d < NB_ASSET; ++d) {
        PNL_SET(spotV, d, spot);
    }
    cout << "CHECK : \n\n";

    cout << "=== Initialisation of Volatility Vector ===\n";
    PnlVect *volatilityV = pnl_vect_create(NB_ASSET);
    for (int d = 0; d < NB_ASSET; ++d) {
        PNL_SET(volatilityV, d, volatility);
    }
    cout << "CHECK : \n\n";


    //Setting past
    PnlMat *past = pnl_mat_create_from_zero(1,NB_ASSET);
    pnl_mat_set_row(past,spotV,0);


    //Setting BSModel car on n'a pas la fonction GetParametersFromStats comme multimonde
    simuIndex->spot = spotV;
    simuIndex->volatility = volatilityV;


    //Initialisation du Pricer MonteCarlo
    int nbSample = 5000;
    PricerGen *pricer = new MonteCarloPricer(maturity,simuIndex,nbSample,(int) maturity);
    assert(pricer != NULL);


    //Initialisation of Call Product

    Asset *asset = new Asset("XX","MyAsset",Change::EUR,trend,spot,volatility);
    assert(asset != NULL);
    Call *call = new Call(pricer,asset,(int) maturity,strike);
    assert(call != NULL);
    cout << " --> \033[1;34m [CHECK]\033[0m\n\n";
    call->Print();
    cout << "\n\n";


    //Computing delta
    PnlVect *delta = pnl_vect_create(NB_ASSET);
    PnlVect *parameters = pnl_vect_create_from_scalar(NB_ASSET,strike);
    cout << "Computing Delta ...\n";
    pricer->Delta(0,past,delta,payOffCall,parameters);

    cout << "Delta sur Call à 0 : \n";
    cout << "\n---> Delta : \n";
    pnl_vect_print(delta);

    //Computing Price
    double ourPrice;
    double ic;
    pricer->Price(0,past,ourPrice,ic,payOffCall,parameters);
    cout << "\n---> Notre Prix du call à 0 : " << ourPrice;


    double prixFF;
    double deltaFF;

    prixFF = pnl_cf_call_bs(spot,strike,maturity,r,0,volatility,&prixFF,&deltaFF);

    cout << "\n---> Prix Formule fermée : " << prixFF;
    cout << "\n---> Delta Formule fermée : " << deltaFF;
    cout << "\n\nCHECK";




    //FREEING Memory
    delete call;
    delete pricer;
    delete simuIndex;
    delete rateModels[0];
    free(rateModels);
    delete asset;
    cout << " --> \033[1;34m [CHECK]\033[0m\n\n";

    cout << "########################################\n\n";
    return EXIT_SUCCESS;

}

