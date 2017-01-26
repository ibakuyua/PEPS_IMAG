#include <stdlib.h>
#include <iostream>
#include <assert.h>
#include "../Pricing/PricerGen.hpp"
#include "../Pricing/MonteCarloPricer.hpp"
#include "../FinancialProducts/Multimonde.hpp"
#include "../SimulationModels/BlackScholesModel.hpp"
#include "../RateModels/ConstantRateModel.hpp"
#include "../Stats/Parser/ParseCSV.h"

using namespace std;


void setParameters(RateModelGen ***rateModels);
void freeParameters(RateModelGen ***rateModels);

#define NB_TIMEVALUE_KNOWN 1 // Must be equal to (int)(t*N/maturity)+2]
#define NB_ASSET 11 // Must be equal to size
#define NB_ECONOMY 6

int main(){

    cout << "\n\n###### TEST OF HEDGING MULTIMONDE ######\n\n";
    cout << "** Instance : ";
    RateModelGen **rateModels;
    setParameters(&rateModels);

    //Initialisation du Modele de BlackScholes
    ModelGen *simuIndex = new BlackScholesModel(NB_ASSET, NB_ECONOMY,rateModels);


    //Initialisation du Pricer MonteCarlo
    int nbSample = 5000;
    int hedgingNb, nbTimeStep;
    hedgingNb = nbTimeStep = (int)Multimonde::maturity; // TODO vérifier qu'on fait bien jour à jour (constructeur multimonde ?)
    PricerGen *pricer = new MonteCarloPricer(
            Multimonde::maturity,simuIndex,nbSample,nbTimeStep);
    assert(pricer != NULL);

    //Import of stats
    ParseCSV *parser = new ParseCSV("../data/dataPEPS.csv");
    StatsFactory *stats = new StatsFactory(parser->inputData);

    //Initialisation of Multimonde Product
    Multimonde *multimonde = new Multimonde(pricer,hedgingNb,stats);
    assert(multimonde != NULL);
    cout << " --> \033[1;34m [CHECK]\033[0m\n\n";
    multimonde->Print();
    cout << "\n\n";


    //Initialisation of Past Matrix
    double pastArray[NB_TIMEVALUE_KNOWN][NB_ASSET] =
            {{SPOT_EUROSTOCK50,SPOT_FTSE,SPOT_P500,SPOT_HANGSENG,
                     SPOT_NIKKEI,SPOT_SPASX200,SPOT_XFTSE,
             SPOT_XP500,SPOT_XHANGSENG,SPOT_XNIKKEI,SPOT_XSPASX200}}; // S(t)

    PnlMat *past = pnl_mat_create(NB_TIMEVALUE_KNOWN,NB_ASSET);
    for (int i = 0; i < NB_TIMEVALUE_KNOWN; ++i) {
        for (int d = 0; d < NB_ASSET; ++d) {
            PNL_MSET(past,i,d,pastArray[i][d]);
        }
    }

    cout << "Voici la matrice past : \n";
    pnl_mat_print(past);

    cout << "Voici la matrice de simulation PATH : \n";
    PnlMat* path = pnl_mat_create_from_zero(NB_TIMEVALUE_KNOWN,NB_ASSET);
    simuIndex->Simulate(Multimonde::maturity,path,nbTimeStep);
    pnl_mat_print(path);

    //Computing delta
    PnlVect *delta = pnl_vect_create(NB_ASSET);
    cout << "Computing Delta ...\n";
    pricer->Delta(0,past,delta,payOffMultimonde21);

    cout << "Delta sur MultiMonde à 0 : \n";
    cout << "\n---> Delta : \n";
    pnl_vect_print(delta);




    //FREEING Memory
    freeParameters(&rateModels);
    cout << " --> \033[1;34m [CHECK]\033[0m\n\n";

    cout << "########################################\n\n";
    return EXIT_SUCCESS;

}


void setParameters(RateModelGen ***rateModels){
    *rateModels = (RateModelGen**) malloc(6 * sizeof(RateModelGen*));
    for (int d = 0; d < 6; ++d)
        (*rateModels)[d] = new ConstantRateModel((Change)d, 0.03/365.);
}

void freeParameters(RateModelGen ***rateModels){
    for (int i = 0; i < 6; ++i) {
        delete (*rateModels)[i];
    }
    delete *rateModels;
}