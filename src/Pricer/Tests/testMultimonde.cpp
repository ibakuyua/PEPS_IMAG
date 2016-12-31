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
    int nbTimeStep = 9000;
    PricerGen * pricer = new MonteCarloPricer(
            Multimonde::maturity,Multimonde::payOff,simuIndex,simuChange,nbSample, nbTimeStep);
    assert(pricer != NULL);
    Multimonde *multimonde = new Multimonde(pricer);
    assert(multimonde != NULL);
    cout << " --> \033[1;34m [CHECK]\033[0m\n\n";
    // Free
    cout << "-) Delete : ";
    delete multimonde;
    delete pricer;
    cout << " --> \033[1;34m [CHECK]\033[0m\n\n";

    cout << "########################################\n\n";
    return EXIT_SUCCESS;
}
