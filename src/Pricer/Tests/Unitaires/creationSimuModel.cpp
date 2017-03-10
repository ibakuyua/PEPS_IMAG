#include <iostream>
#include <assert.h>
#include "../../SimulationModels/ModelGen.hpp"
#include "../../SimulationModels/BlackScholesModel.hpp"
#include "../../RateModels/ConstantRateModel.hpp"

using namespace std;

//Testing creation SimuModel

int main(){

    double FRR = 0.1;

    RateModelGen **rateModels = (RateModelGen**) malloc(1 * sizeof(RateModelGen*));
    rateModels[0] = new ConstantRateModel(Change::EUR,FRR);
    assert(rateModels != NULL && rateModels[0] != NULL);

    ModelGen *bsmodel = new BlackScholesModel(1,1,rateModels);
    assert(bsmodel != NULL);

    cout << "*****    Simulation Model information    *****\n";
    bsmodel->Print();

    return 0;
}