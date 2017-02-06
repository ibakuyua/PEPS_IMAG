#include "PricerGen.hpp"


PricerGen::PricerGen(double theMaturity, ModelGen *simuModel, string name, int nbTimeStep)
        : maturity(theMaturity), simuModel(simuModel), name(name), nbTimeStep(nbTimeStep), scheduleSimulation(NULL)
{
}

PricerGen::PricerGen(double maturity, ModelGen *simuModel, string name, PnlVect *scheduledSteps)
        : maturity(maturity), simuModel(simuModel), name(name), nbTimeStep(scheduledSteps->size), scheduleSimulation(scheduledSteps)
{
}

void PricerGen::PrintPricer() {
    cout << this->name << " with the model : " ;
    this->simuModel->Print();
}

PricerGen::~PricerGen() {
}


