#include <stdlib.h>
#include <iostream>
#include <assert.h>
#include "../../Pricing/PricerGen.hpp"
#include "../../Pricing/MonteCarloPricer.hpp"
#include "../../FinancialProducts/Multimonde.hpp"
#include "../../SimulationModels/BlackScholesModel.hpp"
#include "../../RateModels/ConstantRateModel.hpp"
#include "../../Stats/Parser/ParseCSV.h"
#include "../../Marche.hpp"


using namespace std;

void computePnl(int hedgingNb);
void setParameters(RateModelGen ***rateModels);
void freeParameters(RateModelGen ***rateModels);

int main(int argc, char** argv){
    int hedgingNb(100);
    if (argc > 1)
        hedgingNb = atoi(argv[1]);
    computePnl(hedgingNb);
    return EXIT_SUCCESS;
}

void computePnl(int hedgingNb){

    cout << "\n\n###### TEST OF HEDGING MULTIMONDE (SIMULATION MARKET) ######\n\n";
    cout << "** Instance : ";
    RateModelGen **rateModels;
    setParameters(&rateModels);

    // Scheduled step for simulation
    PnlVect *scheduleSimulation = pnl_vect_create(6);
    LET(scheduleSimulation,0) = NB_DAYSWRK_TO_CONSTATATION_1;
    LET(scheduleSimulation,1) = NB_DAYSWRK_TO_CONSTATATION_2 -  NB_DAYSWRK_TO_CONSTATATION_1;
    LET(scheduleSimulation,2) = NB_DAYSWRK_TO_CONSTATATION_3 -  NB_DAYSWRK_TO_CONSTATATION_2;
    LET(scheduleSimulation,3) = NB_DAYSWRK_TO_CONSTATATION_4 -  NB_DAYSWRK_TO_CONSTATATION_3;
    LET(scheduleSimulation,4) = NB_DAYSWRK_TO_CONSTATATION_5 -  NB_DAYSWRK_TO_CONSTATATION_4;
    LET(scheduleSimulation,5) = NB_DAYSWRK_TO_CONSTATATION_6 -  NB_DAYSWRK_TO_CONSTATATION_5;

    //Initialisation du Modele de BlackScholes
    ModelGen *simuIndex = new BlackScholesModel(11, 6, rateModels);
    assert(simuIndex != NULL);

    //Initialisation du Pricer MonteCarlo
    int nbSample = 50000;
    double maturity = Multimonde::maturity;
    PricerGen *pricer = new MonteCarloPricer(maturity, simuIndex, scheduleSimulation, nbSample);
    assert(pricer != NULL);

    string path = "../data/dataPEPS.csv";
    //Import of stats
    ParseCSV *parser = new ParseCSV(path,2012,10,07,120);
    //ParseCSV *parser = new ParseCSV(path);
    assert(parser != NULL);
    StatsFactory *stats = new StatsFactory(parser->outputData);
    assert(stats != NULL);

    //Initialisation of Multimonde Product
    Multimonde *multimonde = new Multimonde(pricer,hedgingNb,stats);
    assert(multimonde != NULL);

    // Market initialisation
    Marche *marche = Marche::Instance(Change::EUR,multimonde,(int)maturity);
    assert(marche != NULL);
    cout << " --> \033[1;34m [CHECK]\033[0m\n\n";

    multimonde->Print();

/*
 // Forward test
    marche->ImportCotations(CotationTypes::Simulated);
    cout << " \n\nSimulated market : " << marche->cours->m << " quotes : ";
    cout << "--> \033[1;34m [CHECK]\033[0m\n\n";
    cout << "Market : \n\n";
    pnl_mat_print(marche->cours);
    */




    //Backward test
    marche->ImportCotations(CotationTypes::HistoricalMultimonde,2017,12,01,path);
    cout << " \n\nHistorical market : " << marche->cours->m << " quotes : ";
    cout << "--> \033[1;34m [CHECK]\033[0m\n\n";
    cout << "Market : \n\n";
    pnl_mat_print(marche->cours);


    double prixC, prixP, ic;
    double PnL = 0.;
    PnlVect *spotV = pnl_vect_new();
    // At t = 0
    multimonde->UpdatePortfolio(0.);
    multimonde->PricePortfolio(0.,prixP);
    multimonde->PriceProduct(0.,prixC,ic);
    cout << "\n0;" << prixC << ";" << prixP;
    for (int i = 0; i < 11; ++i)
        cout << ";" << GET(multimonde->composition,i);
    /*cout << "\nPrice at t = 0 : " << prixC << " in [ " << prixC - ic/2.
         << " ; " << prixC + ic/2. << " ] ** width : " << ic;
    PnL = prixP - prixC;
    cout << "\nPortfolio price at t = 0 : " << prixP << " PnL [ " << prixP - prixC << " ] \n";
    cout << "\nPnL cumulated [ " << PnL << " ] \n";*/
    // Compute pnl at each date :
    double hedgingStep = maturity / (double)hedgingNb;
    for (double t = hedgingStep; t < maturity; t += hedgingStep) {
        multimonde->PricePortfolio(t,prixP);
        multimonde->PriceProduct(t,prixC,ic);
        multimonde->UpdatePortfolio(t);
        marche->GetCotations(t,spotV);
        cout << "\n" << t << ";" << prixC << ";" << prixP;
        for (int i = 0; i < 11; ++i)
            cout << ";" << GET(multimonde->composition,i);
        /*cout << "\nPrice at t = " << t << " : " << prixC << " in [ " << prixC - ic/2.
             << " ; " << prixC + ic/2. << " ] ** width : " << ic;
        PnL += prixP - prixC;
        cout << "\nPortfolio price at t = " << t << " : " << prixP << " PnL [ " << prixP - prixC << " ] \n";
        cout << "\nPnL cumulated [ " << PnL << " ] \n";*/
    }

    // Final :
    multimonde->PriceProduct(maturity,prixC,ic);
    multimonde->PricePortfolio(maturity,prixP);
    cout << "\n" << maturity << ";" << prixC << ";" << prixP;
    for (int i = 0; i < 11; ++i)
        cout << ";0";
    /*cout << "\n\nPay Off at maturity : " << prixC;
    PnL += prixP - prixC;
    cout << "\nPortfolio price at maturity : " << prixP << " PnL : [ " << prixP - prixC  << " ]";
    cout << "\n\n ------> PnL cumulated at maturity [ " << PnL << " ] \n";*/

    cout << "\n\n** Delete : ";
    pnl_vect_free(&spotV);
    delete multimonde;
    delete pricer;
    delete simuIndex;
    freeParameters(&rateModels);

    cout << " --> \033[1;34m [CHECK]\033[0m\n\n";

    cout << "########################################\n\n";

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