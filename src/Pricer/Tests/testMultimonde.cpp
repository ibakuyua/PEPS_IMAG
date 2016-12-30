#include <stdlib.h>
#include <iostream>
#include <assert.h>
#include "../Pricing/PricerGen.hpp"
#include "../Pricing/MonteCarloPricer.hpp"
#include "../Multimonde.hpp"
#include "../SimulationModels/SimuIndex/BlackScholesIndexModel.hpp"

using namespace std;

int main(){

    cout << "\n\n###### TEST OF THE MULTIMONDE PRICER ######\n\n";
    cout << "1) Instance : ";
    ModelGen *simuIndex /* = new BlackScholesIndexModel(6) */;
    ModelGen *simuChange;
    int nbSample = 50000;
    PricerGen * pricer = new MonteCarloPricer(
            Multimonde::maturity,Multimonde::payOff,simuIndex,simuChange,nbSample);
    assert(pricer != NULL);
    return EXIT_SUCCESS;
}
