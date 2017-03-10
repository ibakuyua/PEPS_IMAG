#include <iostream>
#include <assert.h>
#include "../../RateModels/RateModelGen.hpp"
#include "../../SimulationModels/ModelGen.hpp"
#include "../../SimulationModels/BlackScholesModel.hpp"
#include "../../Pricing/PricerGen.hpp"
#include "../../Pricing/MonteCarloPricer.hpp"
#include "../../RateModels/ConstantRateModel.hpp"

using namespace std;

//Testing creation Pricer

int main(){


    double FRR = 0.1;

    RateModelGen **rateModels = (RateModelGen**) malloc(1 * sizeof(RateModelGen*));
    rateModels[0] = new ConstantRateModel(Change::EUR,FRR);
    assert(rateModels != NULL && rateModels[0] != NULL);

    ModelGen *bsmodel = new BlackScholesModel(1,1,rateModels);
    assert(bsmodel != NULL);


    double maturity = 100;
    int nbTimeStep = 100;
    int nbSample = 1000;

    PricerGen *pricer = new MonteCarloPricer(maturity,bsmodel, nbTimeStep,nbSample);
    assert(pricer != NULL);

    cout << "*****    Pricer information   *****\n";
    pricer->PrintPricer();

    return 0;
}