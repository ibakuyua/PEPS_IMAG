#include "PricerGen.hpp"


PricerGen::PricerGen(double theMaturity, PayOffFunction thePayOff)
        : maturity(theMaturity), payOff(thePayOff)
{
}

PricerGen::~PricerGen() {
}


void PricerGen::setPayOff(PayOffFunction payOffFunction) {
    payOff = payOffFunction;
}
