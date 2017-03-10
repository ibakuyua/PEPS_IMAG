#include <iostream>
#include <assert.h>
#include "../../RateModels/RateModelGen.hpp"
#include "../../RateModels/ConstantRateModel.hpp"

using namespace std;

//Testing creation RateModel

int main(){

    double FRR = 0.1;

    RateModelGen **rateModels = (RateModelGen**) malloc(1 * sizeof(RateModelGen*));
    rateModels[0] = new ConstantRateModel(Change::EUR,FRR);
    assert(rateModels != NULL && rateModels[0] != NULL);

    return 0;
}