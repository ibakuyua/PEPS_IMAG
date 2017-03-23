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
void getDate(int &year, int&month, int&day, double &totalDays);
void computePnl(int hedgingNb);
void setParameters(RateModelGen ***rateModels);
void freeParameters(RateModelGen ***rateModels);

int main(int argc, char** argv){
    int hedgingNb(1500);
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
    string pathResult = "../data/testRebalancementJournalierParamUpdate.csv";

    //Import of stats

    int year = 2010;
    int month = 11;
    int day = 15;
    double totalDays = ((year + (month/12.0))*365 + day);
    double totalDaysOld = totalDays;


    ParseCSV *parser = new ParseCSV(path,year,month,day,120);
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

    //Backward test
    marche->ImportCotations(CotationTypes::HistoricalMultimonde,2017,03,17,path);
    cout << " \n\nHistorical market : " << marche->cours->m << " quotes : ";
    cout << "--> \033[1;34m [CHECK]\033[0m\n\n";
    cout << "Market : \n\n";
    pnl_mat_print(marche->cours);


    double prixC, prixP, ic;
    // At t = 0
    multimonde->UpdatePortfolio(0.);
    multimonde->PricePortfolio(0.,prixP);
    multimonde->PriceProduct(0.,prixC,ic);
    ofstream fichier(pathResult, ios::out | ios::trunc);  // ouverture en écriture avec effacement du fichier ouvert
    string delimiter = ",";
    double pnlAtDate = prixP - prixC;
    double pnl = pnlAtDate;
    if(fichier)
    {
        fichier << year << "-" << month << "-" << day << delimiter << prixC << delimiter << prixP;
        for (int i = 0; i < 11; ++i){
            double tmpDelta = GET(multimonde->composition,i);
            if(tmpDelta != 0){
                fichier << delimiter << tmpDelta;
            }else{
                fichier << delimiter << "0.0";
            }
        }

        fichier << delimiter <<pnlAtDate << delimiter << pnl;
        fichier << '\n';

    }
    else
        cerr << "Impossible d'ouvrir le fichier !" << endl;

    //cout << "\n0;" << prixC << ";" << prixP;
    //for (int i = 0; i < 11; ++i)
    //    cout << ";" << GET(multimonde->composition,i);
    /*cout << "\nPrice at t = 0 : " << prixC << " in [ " << prixC - ic/2.
         << " ; " << prixC + ic/2. << " ] ** width : " << ic;
    PnL = prixP - prixC;
    cout << "\nPortfolio price at t = 0 : " << prixP << " PnL [ " << prixP - prixC << " ] \n";
    cout << "\nPnL cumulated [ " << PnL << " ] \n";*/
    // Compute pnl at each date :
    double hedgingStep = maturity / (double)hedgingNb;
    for (double t = hedgingStep; t < maturity; t += hedgingStep) {


        totalDays += hedgingStep*365./252.;

        //We update stats every 200 days
        if(totalDays - totalDaysOld > 10){
            std::cout << "UPDATE OF STATS !!!!" << std::endl;
            getDate(year,month,day,totalDays);

            parser->Update(path,year,month,day,80);
            assert(parser != NULL);
            //update of stats
            stats->UpdateStatsFactory();
            assert(stats != NULL);

            //Update of the AssetList POINTER !!!!
            multimonde->UpdateAssetListFromStat(stats, pricer->simuModel);
            totalDaysOld = totalDays;
        }


        multimonde->PricePortfolio(t,prixP);
        multimonde->PriceProduct(t,prixC,ic);
        multimonde->UpdatePortfolio(t);
        pnlAtDate = prixP - prixC;
        pnl += pnlAtDate;
        if(fichier)
        {
            getDate(year,month,day,totalDays);
            std::cout << "\n" << year << "-" << month << "-" << day;
            fichier << year << "-" << month << "-" << day  << delimiter<< prixC << delimiter << prixP;

            for (int i = 0; i < 11; ++i){
                double tmpDelta = GET(multimonde->composition,i);
                if(tmpDelta != 0){
                    fichier << delimiter << tmpDelta;
                }else{
                    fichier << delimiter << "0.0";
                }
            }
            fichier << delimiter << prixP - prixC << delimiter << pnl;
            fichier << '\n';

        }
        else
            cerr << "Impossible d'ouvrir le fichier !" << endl;

    }

    // Final :
    multimonde->PriceProduct(maturity,prixC,ic);
    multimonde->PricePortfolio(maturity,prixP);
    pnlAtDate = prixP - prixC;
    pnl += pnlAtDate;
    if(fichier)
    {
        totalDays += hedgingStep*365./252.;
        getDate(year,month,day,totalDays);
        fichier << year << "-" << month << "-" << day  << delimiter<< prixC << delimiter << prixP;

        for (int i = 0; i < 11; ++i){
            fichier << delimiter << "0.0";
        }

        fichier << delimiter << prixP - prixC << delimiter << pnl;
        fichier << '\n';
        fichier.close();
    }
    else
        cerr << "Impossible d'ouvrir le fichier !" << endl;




    //DELETE
    cout << "\n\n** Delete : ";
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
        (*rateModels)[d] = new ConstantRateModel((Change)d, 0.03/252.);
}

void freeParameters(RateModelGen ***rateModels){
    for (int i = 0; i < 6; ++i) {
        delete (*rateModels)[i];
    }
    delete *rateModels;
}

void getDate(int &year, int&month, int&day, double &totalDays){

    year = (int)totalDays/365;
    month = ((int)totalDays - year*365)/30;
    day = (int)totalDays - year*365 - month*30;
    if(month == 0){
        year = year - 1;
        month = 12;
        day = (int)totalDays - year*365 - month*30;
        day = (day > 30) ? 30 : day;
    }
    if(day == 0){
        if(month == 2){
            day = 28;
        }else if(month > 1){
            day = 30;
            month -= 1;
        }else{
            day = 30;
            month = 12;
            year = year - 1;
        }
    }else if(day > 28){
        if (month == 2){
            day = 28;
        }
    }

}