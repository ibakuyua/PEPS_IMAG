#include "PricerGen.hpp"


PricerGen::PricerGen(double theMaturity, PayOffFunction thePayOff)
        : maturity(theMaturity), payOff(thePayOff)
{
}

PricerGen::~PricerGen() {
}
