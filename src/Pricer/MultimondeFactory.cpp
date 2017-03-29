#include <assert.h>
#include "MultimondeFactory.hpp"
#include "RateModels/RateModelGen.hpp"
#include "SimulationModels/BlackScholesModel.hpp"
#include "ProductInfo/infoMultimonde.hpp"
#include "Pricing/MonteCarloPricer.hpp"
#include "Stats/Parser/ParseCSV.h"
#include "FinancialProducts/Multimonde.hpp"
#include "RateModels/ConstantRateModel.hpp"
#include "Marche.hpp"

void setParameters(RateModelGen ***rateModels);
void freeParameters(RateModelGen ***rateModels);

void MultimondeFactory::Price(double t, int year, int month, int day ,double &price, double &std) {
    // TODO : mettre les bons taux d'intérêts
    cout << "#### Pricing at t = " << t << " at date " << year << "/" << month << "/" << day;
    RateModelGen **rateModels;
    setParameters(&rateModels);
    ModelGen *simuIndex = new BlackScholesModel(11, 6, rateModels);
    int nbSample = 50000;
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
    ParseCSV *parser = new ParseCSV("C:\\Users\\Paul\\Documents\\Visual Studio 2013\\Projects\\ProjetEvaluationProduitStructure21\\data\\dataPEPS.csv",year,month,day,180);
    StatsFactory *stats = new StatsFactory(parser->outputData);
    Multimonde *multimonde = new Multimonde(pricer,hedgingNb,stats);
    assert(multimonde != NULL);
    multimonde->Print();
    cout << "\n\n";

    // Market initialisation
    Marche *marche = Marche::Instance(Change::EUR,multimonde,(int)Multimonde::maturity);
    assert(marche != NULL);
    cout << " --> \033[1;34m [CHECK]\033[0m\n\n";
    marche->ImportCotations(CotationTypes::HistoricalMultimonde,year,month,day,"C:\\Users\\Paul\\Documents\\Visual Studio 2013\\Projects\\ProjetEvaluationProduitStructure21\\data\\dataPEPS.csv");
    cout << " \n\nHistorical market : " << marche->cours->m << " quotes : ";
    cout << "--> \033[1;34m [CHECK]\033[0m\n\n";
    cout << "Market : \n\n";
    pnl_mat_print(marche->cours);

    cout << "Computing Price ...\n";
    multimonde->PriceProduct(t, price, std);
    std = std / 3.92;
    cout << "\n--> Price : " << price;
    cout << "\n--> Ic : [ " << (price - std * 1.96) << " ; " << (price + std * 1.96) << " ]"  ;
    cout << "\n\n--> Forward price " << price * exp(rateModels[0]->GetIntegralRate(t,Multimonde::maturity));
    // Free
    cout << "\n\n** Delete : ";
    delete multimonde;
    delete pricer;
    freeParameters(&rateModels);
    cout << " --> \033[1;34m [CHECK]\033[0m\n\n";

    cout << "########################################\n\n";


}

void MultimondeFactory::Hedge(double t, int year, int month, int day, double *compo, double *std) {
    // TODO : mettre les bons taux d'intérêts
    cout << "#### Hedging at t = " << t << " at date " << year << "/" << month << "/" << day;
    RateModelGen **rateModels;
    setParameters(&rateModels);
    ModelGen *simuIndex = new BlackScholesModel(11, 6, rateModels);
    int nbSample = 50000;
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
    ParseCSV *parser = new ParseCSV("C:\\Users\\Paul\\Documents\\Visual Studio 2013\\Projects\\ProjetEvaluationProduitStructure21\\data\\dataPEPS.csv",year,month,day,180);
    StatsFactory *stats = new StatsFactory(parser->outputData);
    Multimonde *multimonde = new Multimonde(pricer,hedgingNb,stats);
    assert(multimonde != NULL);
    multimonde->Print();
    cout << "\n\n";

    // Market initialisation
    Marche *marche = Marche::Instance(Change::EUR,multimonde,(int)Multimonde::maturity);
    assert(marche != NULL);
    cout << " --> \033[1;34m [CHECK]\033[0m\n\n";
    marche->ImportCotations(CotationTypes::HistoricalMultimonde,year,month,day,"C:\\Users\\Paul\\Documents\\Visual Studio 2013\\Projects\\ProjetEvaluationProduitStructure21\\data\\dataPEPS.csv");
    cout << " \n\nHistorical market : " << marche->cours->m << " quotes : ";
    cout << "--> \033[1;34m [CHECK]\033[0m\n\n";
    cout << "Market : \n\n";
    pnl_mat_print(marche->cours);

    cout << "Computing Portfolio ...\n";
    multimonde->UpdatePortfolio(t);
    for (int i = 0; i < 12; ++i) {
        compo[i] = GET(multimonde->composition,i);
        std[i] = GET(multimonde->icComposition, i) / 3.96;
    }
    cout << "\n--> Composition :  \n";
    pnl_vect_print(multimonde->composition);
    cout << "\n--> Ic : \n";
    pnl_vect_print(multimonde->icComposition);
    // Free
    cout << "\n\n** Delete : ";
    delete multimonde;
    delete pricer;
    freeParameters(&rateModels);
    cout << " --> \033[1;34m [CHECK]\033[0m\n\n";

    cout << "########################################\n\n";
}

void MultimondeFactory::BackTest(int hedgingNb, int MCnb, char *path, double discrStep) {

}

void MultimondeFactory::ForwardTest(int hedgingNb, int MCnb, char *path, double discrStep) {
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
    int nbSample = MCnb;
    double maturity = Multimonde::maturity;
    PricerGen *pricer = new MonteCarloPricer(maturity, simuIndex, scheduleSimulation, nbSample);
    assert(pricer != NULL);

    //Import of stats
    int year = 2010;
    int month = 11;
    int day = 15;

    ParseCSV *parser = new ParseCSV("C:\\Users\\Paul\\Documents\\Visual Studio 2013\\Projects\\ProjetEvaluationProduitStructure21\\data\\dataPEPS.csv",year,month,day,80);
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

 // Forward test
    marche->ImportCotations(CotationTypes::Simulated);
    cout << " \n\nSimulated market : " << marche->cours->m << " quotes : ";
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

    double pnlAtDate = prixP - prixC;
    double pnl = pnlAtDate;

    ofstream fichier(path, ios::out | ios::trunc);  // ouverture en écriture avec effacement du fichier ouvert
    string delimiter = ",";
    if(fichier)
    {
        fichier << year << "-" << month << "-" << day << delimiter << prixC << delimiter << prixP;
        for (int i = 0; i < 11; ++i){
            double tmpDelta = GET(multimonde->composition,i);
            fichier << delimiter << tmpDelta;
        }

        fichier << delimiter <<pnlAtDate << delimiter << pnl;
        fichier << '\n';

    }
    else
        cerr << "Impossible d'ouvrir le fichier !" << endl;

    // Compute pnl at each date :
    double totalDays = ((year + (month/12.0))*365 + day);
    double hedgingStep = maturity / (double)hedgingNb;
    for (double t = hedgingStep; t < maturity; t += hedgingStep) {
        totalDays +=  hedgingStep * 365./252.;
        multimonde->PricePortfolio(t,prixP);
        multimonde->PriceProduct(t,prixC,ic);
        multimonde->UpdatePortfolio(t);
        pnlAtDate = prixP - prixC;
        pnl += pnlAtDate;
        if(fichier)
        {
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
                    month --;
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


            std::cout << "\n" << year << "-" << month << "-" << day;

            fichier << year << "-" << month << "-" << day  << delimiter<< prixC << delimiter << prixP;

            for (int i = 0; i < 11; ++i){
                double tmpDelta = GET(multimonde->composition,i);
                fichier << delimiter << tmpDelta;
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
	if (fichier)
	{
		totalDays += hedgingStep*365. / 252.;
		year = (int)totalDays / 365;
		month = ((int)totalDays - year * 365) / 30;
		day = (int)totalDays - year * 365 - month * 30;
		if (month == 0){
			year = year - 1;
			month = 12;
			day = (int)totalDays - year * 365 - month * 30;
			day = (day > 30) ? 30 : day;
		}
		if (day == 0){
			if (month == 2){
				day = 28;
			}
			else if (month > 1){
				day = 30;
				month -= 1;
			}
			else{
				day = 30;
				month = 12;
				year = year - 1;
			}
		}
		else if (day > 28){
			if (month == 2){
				day = 28;
			}
		}
		fichier << year << "-" << month << "-" << day << delimiter << prixC << delimiter << prixP;

		for (int i = 0; i < 11; ++i){
			fichier << delimiter << "0.0";
		}

		fichier << delimiter << prixP - prixC << delimiter << pnl;
		fichier << '\n';
	}
    else
        cerr << "Impossible d'ouvrir le fichier !" << endl;


 //   if(fichier)
 //   {
 //       fichier.close();
 //   }
 //   else
 //       cerr << "Impossible d'ouvrir le fichier !" << endl;

	//
 //   pnl_vect_free(&spotV);
 //   delete multimonde;
	//delete pricer;
 //   delete simuIndex;
 //   freeParameters(&rateModels);


}


void setParameters(RateModelGen ***rateModels){
    *rateModels = (RateModelGen**) malloc(6 * sizeof(RateModelGen*));

    (*rateModels) [0] = new ConstantRateModel((Change) 0, RFF_FRANCE/365.);
    (*rateModels) [1] = new ConstantRateModel((Change) 1, RFF_UK/365.);
    (*rateModels) [2] = new ConstantRateModel((Change) 2, RFF_US/365.);
    (*rateModels) [3] = new ConstantRateModel((Change) 3, RFF_CHINA/365.);
    (*rateModels) [4] = new ConstantRateModel((Change) 4, RFF_JAPAN/365.);
    (*rateModels) [5] = new ConstantRateModel((Change) 5, RFF_AUSTRALIA/365.);

}

void freeParameters(RateModelGen ***rateModels){
    for (int i = 0; i < 6; ++i) {
        delete (*rateModels)[i];
    }
    delete *rateModels;
}
