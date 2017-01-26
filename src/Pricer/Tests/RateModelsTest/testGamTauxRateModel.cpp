#include <iostream>
#include <assert.h>
#include <stdexcept>
#include "../../RateModels/GamTauxRateModel.hpp"

using namespace std;

int main(){
    cout << "\n\n###### TEST OF CONSTANT RATE MODEL ######\n\n";
    cout << "1) Instance : ";
    GamTauxRateModel *rateModel = new GamTauxRateModel(Change::EUR, 0.03, 0.01);
    assert(rateModel != NULL);
    cout << " --> \033[1;34m [CHECK]\033[0m\n\n";
    cout << " == Paramètres : \n";
    cout << "    Rinf = " << rateModel->r_inf << " S = "
         << rateModel->spread << " gamma = "
         << rateModel->gamma << " a = " << rateModel->a;
    assert(rateModel->r_inf == 0.03);
    assert(rateModel->spread == 0.01);
    assert(rateModel->gamma == 0.01);
    assert(rateModel->a == 0.4);
    cout << " \n\n --> \033[1;34m [CHECK]\033[0m\n\n";
    cout << "2) Courbe des taux moyen [t | R(0,t)] : \n";
    double t;
    int i;
    for (t = 0.1, i = 0; t < 10 ; t+=0.5, i++) {
        if (i % 5 == 0)
            cout << "\n";
        cout << "[ " << t << " | " << rateModel->GetRate(t) << " ]  ";
    }
    cout << "\n\n --> \033[1;34m [CHECK]\033[0m\n\n";
    cout << "3) Exception en T=0 : ";
    try {
        rateModel->GetRate(0);
        cout << " \n\n --> \033[1;31m [FAIL]\033[0m\n\n";

    }catch (domain_error d) {
        cout << " \n\n --> \033[1;34m [CHECK]\033[0m\n\n";
    }

    cout << "##### FIN DU TEST #####\n\n";
    return EXIT_SUCCESS;
}