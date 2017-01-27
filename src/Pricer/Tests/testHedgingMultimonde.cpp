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
void computePnl();
double hedging(PricerGen *pricer, double& V_iMinus1, RateModelGen ***rateModels,
               PnlVect *deltas_iMinus1, PnlVect *Stau_i, PnlMat *past, double t,double marketStep, double &pi);
void setParameters(RateModelGen ***rateModels);
void freeParameters(RateModelGen ***rateModels);

#define NB_TIMEVALUE_KNOWN 1 // Must be equal to (int)(t*N/maturity)+2]
#define NB_ASSET 11 // Must be equal to size
#define NB_ECONOMY 6

int main(){

    computePnl();
    return EXIT_SUCCESS;

}

void computePnl(){

    cout << "\n\n###### TEST OF HEDGING MULTIMONDE ######\n\n";
    cout << "** Instance : ";
    RateModelGen **rateModels;
    setParameters(&rateModels);

    PnlVect *scheduleSimulation = pnl_vect_create(6);
    LET(scheduleSimulation,0) = NB_DAYS_TO_CONSTATATION_1;
    LET(scheduleSimulation,1) = NB_DAYS_TO_CONSTATATION_2 -  NB_DAYS_TO_CONSTATATION_1;
    LET(scheduleSimulation,2) = NB_DAYS_TO_CONSTATATION_3 -  NB_DAYS_TO_CONSTATATION_2;
    LET(scheduleSimulation,3) = NB_DAYS_TO_CONSTATATION_4 -  NB_DAYS_TO_CONSTATATION_3;
    LET(scheduleSimulation,4) = NB_DAYS_TO_CONSTATATION_5 -  NB_DAYS_TO_CONSTATATION_4;
    LET(scheduleSimulation,5) = NB_DAYS_TO_CONSTATATION_6 -  NB_DAYS_TO_CONSTATATION_5;

    //Initialisation du Modele de BlackScholes
    ModelGen *simuIndex = new BlackScholesModel(NB_ASSET, NB_ECONOMY, rateModels);

    //Initialisation du Pricer MonteCarlo
    int nbSample = 5000;
    int hedgingNb, nbTimeStep;
    hedgingNb = 30;
    PricerGen *pricer = new MonteCarloPricer(Multimonde::maturity, simuIndex, scheduleSimulation, nbSample);
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





            // Market initialisation
    cout << " --- Simulation du marché --- \n";
    PnlMat *market = pnl_mat_create_from_zero(hedgingNb + 1, NB_ASSET);
    simuIndex->SimulateMarket(Multimonde::maturity,market,hedgingNb);
    cout << " --> \033[1;34m [CHECK]\033[0m\n\n";


    // Création du portefeuille et du vecteur de PnL (PnL à chaque date)
    PnlVect *pnlAtDate = pnl_vect_create(hedgingNb + 1);

    // Initialisation
    //Delta prec
    PnlVect *deltas_iMinus1 = pnl_vect_create(NB_ASSET);
    //Stau courant, les prix au temps i des actifs
    PnlVect *Stau_i = pnl_vect_new();
    pnl_mat_get_row(Stau_i, market, 0);


    double price,ic;
    double marketStep = Multimonde::maturity/(double)hedgingNb;
    cout << "Computing Price and Delta....\n";
    pricer->Price(0,past,price,ic,payOffMultimonde21);
    pricer->Delta(0,past,deltas_iMinus1,payOffMultimonde21);
    std::cout << "Le prix en 0 est de : " << price << std::endl;
    std::cout << "Les delta en 0 sont de : " << std::endl;
    pnl_vect_print(deltas_iMinus1);
    std::cout << std::endl;
    double V_iMinus1 = price  - pnl_vect_scalar_prod(deltas_iMinus1,Stau_i);
    LET(pnlAtDate, 0) = 0; // Par construction



    // Foreach date
    int iN = 1;
    for (int i = 1; i < pnlAtDate->size; ++i) {
        double t = i * marketStep;
        pnl_mat_get_row(Stau_i, market, i);

        if ((i * marketStep) >= iN * Multimonde::maturity/ simuIndex->assetNb){
            pnl_mat_add_row(past, past->m, Stau_i);
            iN++;
        }else{
            pnl_mat_set_row(past, Stau_i, past->m - 1);
        }

        LET(pnlAtDate, i) = hedging(pricer, V_iMinus1, &rateModels, deltas_iMinus1, Stau_i, past, t, marketStep, price);



        // TEMPORARY INFORMATIONS
        // TODO : delete on time
        std::cout << "Le pnl de la date courante est de :" << GET(pnlAtDate,i) << std::endl;
    }

    // Display result
    cout << "\n-----> Pay-Off [ " << price << " ]";
    cout << "\n-----> PnL [ " << GET(pnlAtDate, hedgingNb) << " ]\n";
    cout << "\n\n\n Marché : \n\n";
    pnl_mat_print(market);
    cout << "\n\n PnL at date : \n";
    pnl_vect_print(pnlAtDate);


    //FREEING Memory
    delete multimonde;
    delete pricer;
    freeParameters(&rateModels);
    cout << " --> \033[1;34m [CHECK]\033[0m\n\n";

    cout << "########################################\n\n";

}


double hedging(PricerGen *pricer, double& V_iMinus1, RateModelGen ***rateModels, PnlVect *deltas_iMinus1,
               PnlVect *Stau_i, PnlMat *past, double t, double marketStep, double &price){

    double capitalizationFactor = exp(-(*rateModels)[0]->GetIntegralRate(t,t+marketStep));

    PnlVect *delta_i = pnl_vect_create(deltas_iMinus1->size);
    pricer->Delta(t,past,delta_i,payOffMultimonde21);

    double deltas_iStau_i = pnl_vect_scalar_prod(delta_i,Stau_i);
    double deltas_iMinus1Stau_i = pnl_vect_scalar_prod(deltas_iMinus1,Stau_i);

    double Vi = V_iMinus1 * capitalizationFactor - (deltas_iStau_i - deltas_iMinus1Stau_i);

    double ic;
    pricer->Price(t,past,price,ic,payOffMultimonde21);
    pnl_vect_clone(deltas_iMinus1,delta_i);
    double pi = deltas_iStau_i + Vi;

    V_iMinus1 = Vi;

    return pi - price;
}


void setParameters(RateModelGen ***rateModels){
    *rateModels = (RateModelGen**) malloc(6 * sizeof(RateModelGen*));
    for (int d = 0; d < 6; ++d)
        (*rateModels)[d] = new ConstantRateModel((Change)d, 0.03/252.);
}

void freeParameters(RateModelGen ***rateModels){
    for (int i = 0; i < 6; ++i) {
        delete (*rateModels)[i];
    }
    delete *rateModels;
}