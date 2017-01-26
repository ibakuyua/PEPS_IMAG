#include "PricerGen.hpp"


PricerGen::PricerGen(double theMaturity, ModelGen *simuModel, string name, int nbTimeStep)
        : maturity(theMaturity), simuModel(simuModel), name(name), nbTimeStep(nbTimeStep)
{
}


void PricerGen::PrintPricer() {
    cout << this->name << " with the model : " ;
    this->simuModel->Print();
}

PricerGen::~PricerGen() {
}


