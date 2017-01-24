#include <iostream>
#include <assert.h>
#include <math.h>
#include "../../FinancialProducts/PayOffs/PayOffFunctions.hpp"
#include "../../ProductInfo/infoMultimonde.hpp"
#include "../../RateModels/ConstantRateModel.hpp"

using namespace std;

int main(){
   cout << "\n\n#### TEST OF MULTIMONDE PAY OFFs ####\n\n";
    int constatationIndexes[6] =
            {
                    (int)NB_DAYSWRK_TO_CONSTATATION_1,
                    (int)NB_DAYSWRK_TO_CONSTATATION_2,
                    (int)NB_DAYSWRK_TO_CONSTATATION_3,
                    (int)NB_DAYSWRK_TO_CONSTATATION_4,
                    (int)NB_DAYSWRK_TO_CONSTATATION_5,
                    (int)NB_DAYSWRK_TO_CONSTATATION_6
            };
    // RateModel initialisation
    map<Change,RateModelGen*> rateModels;
    for (int r = 0; r < 6; ++r) {
        rateModels[(Change)r] = new ConstantRateModel((Change)r,0.03/365.);
    }
    // Path initialisation
    PnlMat *path = pnl_mat_create_from_scalar((int)NB_DAYS_TO_MATURITY + 1,11,1);
    // Spot definition
    MLET(path,0,0) = 100;
    for (int i = 1; i < 6; ++i) {
        MLET(path,0,i) = 100;
        MLET(path,0,i+5) = 1 * exp(rateModels[(Change)i]->GetIntegralRate(0,(int)NB_DAYS_TO_MATURITY));
    }
    // Constatation date definition
    //MLET(path,(int)NB_DAYSWRK_TO_CONSTATATION_1,)

    return EXIT_SUCCESS;
}
