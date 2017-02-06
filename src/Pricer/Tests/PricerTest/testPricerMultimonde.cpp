#include <stdlib.h>
#include <iostream>
#include <assert.h>
#include "../../Pricing/PricerGen.hpp"
#include "../../Pricing/MonteCarloPricer.hpp"
#include "../../FinancialProducts/Multimonde.hpp"
#include "../../SimulationModels/BlackScholesModel.hpp"
#include "../../RateModels/ConstantRateModel.hpp"
#include "../../Stats/Parser/ParseCSV.h"

using namespace std;

void setParameters(RateModelGen ***rateModels);
void freeParameters(RateModelGen ***rateModels);

int main(){

    //TODO : faire les const dans toutes les fonctions là où ça va bien

    cout << "\n\n###### TEST OF THE MULTIMONDE PRICER ######\n\n";
    cout << "** Instance : ";
    RateModelGen **rateModels;
    setParameters(&rateModels);
    ModelGen *simuIndex = new BlackScholesModel(11, 6, rateModels);
    int nbSample = 5000;
    int hedgingNb = 0;

    PnlVect *scheduleSimulation = pnl_vect_create(6);
    LET(scheduleSimulation,0) = NB_DAYSWRK_TO_CONSTATATION_1;
    LET(scheduleSimulation,1) = NB_DAYSWRK_TO_CONSTATATION_2 -  NB_DAYSWRK_TO_CONSTATATION_1;
    LET(scheduleSimulation,2) = NB_DAYSWRK_TO_CONSTATATION_3 -  NB_DAYSWRK_TO_CONSTATATION_2;
    LET(scheduleSimulation,3) = NB_DAYSWRK_TO_CONSTATATION_4 -  NB_DAYSWRK_TO_CONSTATATION_3;
    LET(scheduleSimulation,4) = NB_DAYSWRK_TO_CONSTATATION_5 -  NB_DAYSWRK_TO_CONSTATATION_4;
    LET(scheduleSimulation,5) = NB_DAYSWRK_TO_CONSTATATION_6 -  NB_DAYSWRK_TO_CONSTATATION_5;

    PricerGen * pricer = new MonteCarloPricer(Multimonde::maturity, simuIndex, scheduleSimulation, nbSample);
    assert(pricer != NULL);
    ParseCSV *parser = new ParseCSV("../data/dataPEPS.csv");
    StatsFactory *stats = new StatsFactory(parser->inputData);
    Multimonde *multimonde = new Multimonde(pricer,hedgingNb,stats);
    assert(multimonde != NULL);
    cout << " --> \033[1;34m [CHECK]\033[0m\n\n";
    multimonde->Print();
    cout << "\n\n";
    double price, ic;
    cout << "Computing Price ...\n";
    multimonde->PriceProduct(0, price, ic);
    cout << "\n--> Price : " << price;
    cout << "\n--> Ic : [ " << (price - ic/2) << " ; " << (price + ic/2) << " ]"  ;
    cout << "\n\n--> Forward price " << price * exp(rateModels[0]->GetIntegralRate(0,Multimonde::maturity));
    // Free
    cout << "\n\n** Delete : ";
    delete multimonde;
    delete pricer;
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