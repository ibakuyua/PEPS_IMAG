#include "PricerGen.hpp"


PricerGen::PricerGen(double theMaturity, ModelGen *simuModel, string name)
        : maturity(theMaturity), simuModel(simuModel), name(name)
{
}


void PricerGen::PrintPricer() {
    cout << this->name << " with the model : " ;
    this->simuModel->Print();
}

PricerGen::~PricerGen() {
}


