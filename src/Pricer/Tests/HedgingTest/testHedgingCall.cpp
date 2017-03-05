#include <iostream>
#include <assert.h>
#include "../../FinancialProducts/Asset/Asset.hpp"
#include "../../Marche.hpp"
#include "../../RateModels/ConstantRateModel.hpp"
#include "../../SimulationModels/BlackScholesModel.hpp"
#include "../../FinancialProducts/Call.hpp"
#include "../../Pricing/MonteCarloPricer.hpp"
#include "pnl/pnl_finance.h"

using namespace std;

/////// TEST HEDGING CALL BY SIMULATION ////////
int main(){
    double spot = 100.;
    double vol = 0.07;
    double FRR = 0.03;
    double maturity = 20.;
    double strike = 100.;
    //int simuNb = (int)(maturity/2);
    PnlVect *scheduleSimulation = pnl_vect_create(2);
    LET(scheduleSimulation,0) = 13.;
    LET(scheduleSimulation,1) = 7.;
    int sampleNb = 100000;
    cout << "** Instance : ";
    Asset * asset = new Asset("BNP","BNP",Change::EUR,0.03,spot,vol);
    assert(asset != NULL);
    RateModelGen **rateModels = (RateModelGen**) malloc(1 * sizeof(RateModelGen*));
    rateModels[0] = new ConstantRateModel(Change::EUR,FRR);
    assert(rateModels != NULL && rateModels[0] != NULL);
    ModelGen *modelBS = new BlackScholesModel(1, 1, rateModels);
    assert(modelBS != NULL);
    //PricerGen *pricerMC = new MonteCarloPricer(maturity, modelBS, simuNb, sampleNb);
    PricerGen *pricerMC = new MonteCarloPricer(maturity, modelBS, scheduleSimulation, sampleNb);
    assert(pricerMC != NULL);
    ProductGen *call = new Call(pricerMC,asset,(int)maturity,strike);
    assert(call != NULL);
    Marche *marche = Marche::Instance(Change::EUR,call,20);
    assert(marche != NULL);
    cout << " --> \033[1;34m [CHECK]\033[0m\n\n";

    call->Print();

    marche->ImportCotations(CotationTypes::Simulated);
    cout << " \n\nSimulated market : \n\n";
    pnl_mat_print(marche->cours);
    cout << "--> \033[1;34m [CHECK]\033[0m\n\n";

    double prixC, prixCFF, prixP, ic;
    double PnL = 0;
    PnlVect *spotV = pnl_vect_new();
    // At t = 0
    call->UpdatePortfolio(0.);
    call->PricePortfolio(0.,prixP);
    call->PriceProduct(0.,prixC,ic);
    prixCFF = pnl_bs_call(spot,strike,maturity,FRR,0.,vol);
    ///*Uncomment for excel analysis */cout << "\n0;" << prixC << ";" << prixP << ";" << GET(call->composition,0);
    cout << "\nPrice at 0 : " << prixC << " in [ " << prixC - ic/2.
         << " ; " << prixC + ic/2. << " ] ** Real = " << prixCFF;
    cout << "\nPortfolio price at 0 : " << prixP << " PnL [ " << prixP - prixC << " ] \n";
    PnL = prixP - prixC;
    cout << "PnL cumlulated : [ " << PnL << " ]\n";
    // TODO : est-ce problème car on fait deux fois price product : une fois pour PriceProduct une fois pour Update d'où un pnl != 0 à 0
    // Compute pnl at each date :
    for (double t = 1.; t < maturity; ++t) {
        call->PricePortfolio(t,prixP);
        call->PriceProduct(t,prixC,ic);
        call->UpdatePortfolio(t);
        marche->GetCotations(t,spotV);
        prixCFF = pnl_bs_call(GET(spotV,0),strike,maturity-t,FRR,0.,vol);
        ///*Uncomment for excel analysis */cout << "\n" << t << ";" << prixC << ";" << prixP << ";" << GET(call->composition,0);
        cout << "\nPrice at " << t << " : " << prixC << " in [ " << prixC - ic/2.
             << " ; " << prixC + ic/2. << " ] ** Real = " << prixCFF;
        cout << "\nPortfolio price at " << t << " : " << prixP << " PnL [ " << prixP - prixC << " ] \n";
        PnL += prixP - prixC;
        cout << "PnL cumlulated : [ " << PnL << " ]\n";
    }

    // Final :
    call->PriceProduct(maturity,prixC,ic);
    call->PricePortfolio(maturity,prixP);
    cout << "\n\nPay Off at maturity : " << prixC;
    cout << "\nPortfolio price at maturity : " << prixP << " PnL : [ " << prixP - prixC << " ]";
    PnL += prixP - prixC;
    cout << "\n\n ----> PnL cumlulated : [ " << PnL << " ]\n";

    cout << "\n\n** Delete : ";
    pnl_vect_free(&spotV);
    delete call;
    delete pricerMC;
    delete modelBS;
    delete rateModels[0];
    free(rateModels);
    delete asset;
    cout << " --> \033[1;34m [CHECK]\033[0m\n\n";

    return EXIT_SUCCESS;
}
