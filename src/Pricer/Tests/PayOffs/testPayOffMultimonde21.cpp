#include <iostream>
#include <assert.h>
#include <math.h>
#include "../../FinancialProducts/PayOffs/PayOffFunctions.hpp"
#include "../../ProductInfo/infoMultimonde.hpp"
#include "../../RateModels/ConstantRateModel.hpp"

using namespace std;

int main(){
   cout << "\n\n#### TEST OF MULTIMONDE PAY OFFs ####\n\n";
    cout << "** Initialisation of path and rate models : ";
    // RateModel initialisation
    map<Change,RateModelGen*> rateModels;
    for (int r = 0; r < 6; ++r) {
        rateModels[(Change)r] = new ConstantRateModel((Change)r,0.03/365.);
    }
    // Path initialisation
    PnlMat *path = pnl_mat_create_from_scalar((int)NB_DAYSWRK_TO_MATURITY + 1,11,1);
    assert(path != NULL);
    // Spot definition
    MLET(path,0,0) = 100;
    for (int i = 1; i < 6; ++i) {
        MLET(path,0,i) = 100;
        MLET(path,0,i+5) = 1 * exp(rateModels[(Change)i]->GetIntegralRate(0,(int)NB_DAYSWRK_TO_MATURITY));
    }
    // Constatation dates definition
    int constatationIndexes[6] =
            {
                    (int)NB_DAYSWRK_TO_CONSTATATION_1,(int)NB_DAYSWRK_TO_CONSTATATION_2,
                    (int)NB_DAYSWRK_TO_CONSTATATION_3,(int)NB_DAYSWRK_TO_CONSTATATION_4,
                    (int)NB_DAYSWRK_TO_CONSTATATION_5,(int)NB_DAYSWRK_TO_CONSTATATION_6
            };
    double values[6][6] =
            {
                    {120.,130.,120.,110.,105.,110.}, // --> Indice 2 avec perf 30 % capé à 15%
                    {110.,108.,107.,90.,108.,109.}, // --> Indice 1 avec perf 10 %
                    {130.,110.,98.,90.,80.,87.}, // --> Indice 3 avec perf -2%
                    {110.,110.,100.,70.,80.,60.}, // --> Indice 5 avec perf -20% floré à -15%
                    {120.,160.,110.,105.,120.,100.}, // --> Indice 4 avec perf 5%
                    {110.,90.,80.,105.,110.,97.} // --> Indice 6 avec perf -3%
            };

    for (int d = 0; d < 6; ++d) { // Chaque date de constatation
        int date = constatationIndexes[d];
        MLET(path,date,0) = values[d][0];
        for (int i = 1; i < 6; ++i) { // Chaque indice
            MLET(path,date,i) = values[d][i];
            double test = exp(rateModels[(Change)i]->GetIntegralRate(date,(int)NB_DAYSWRK_TO_MATURITY));
            MLET(path,date,i+5) = test;

        }
    }
    cout << " --> \033[1;34m [CHECK]\033[0m\n\n";

    // Compute payOff :
    double payOff = payOffMultimonde21(path,NULL,rateModels);
    cout << "** Computed payoff : " << payOff << "  [True value : " << 110 << "] --> ";
    assert(fabs(payOff - 110.) < 0.000001);
    cout << " --> \033[1;34m [CHECK]\033[0m\n\n";
    cout << "#########################################\n\n";

    return EXIT_SUCCESS;
}
