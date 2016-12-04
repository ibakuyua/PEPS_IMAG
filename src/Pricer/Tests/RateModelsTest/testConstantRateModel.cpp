#include <iostream>
#include <assert.h>
#include "../../RateModels/ConstantRateModel.hpp"

using namespace std;

int main(){
    cout << "\n\n###### TEST OF CONSTANT RATE MODEL ######\n\n";
    cout << "1) Instance : ";
    RateModelGen *rateModel = new ConstantRateModel();
    assert(rateModel != NULL);
    cout << " --> \033[1;34m [CHECK]\033[0m\n\n";
    cout << " == Paramètres : r = " << ((ConstantRateModel*)rateModel)->parameter << "\n\n";
    cout << "2) Courbe des taux [t | R(t)] : \n";
    double t;
    int i;
    for (t = 0, i = 0; t < 5; t+=0.5, i++) {
        if (i % 5 == 0)
            cout << "\n";
        cout << "[ " << t << " | " << rateModel->GetRate(t) << " ]  ";
    }
    assert(rateModel->GetRate(2) == 0.02);
    cout << "\n\n --> \033[1;34m [CHECK]\033[0m\n";
    cout << "\n\n##### FIN DU TEST #####\n\n";
    return EXIT_SUCCESS;
}
