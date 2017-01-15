#include "PricerGen.hpp"


PricerGen::PricerGen(double theMaturity, ModelGen *simuModel)
        : maturity(theMaturity), simuModel(simuModel)
{
}

PricerGen::~PricerGen() {
}


