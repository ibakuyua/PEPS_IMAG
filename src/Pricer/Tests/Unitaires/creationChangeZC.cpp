#include <iostream>
#include <assert.h>
#include "../../FinancialProducts/Asset/ChangeZC.hpp"
#include "../../ProductInfo/infoMultimonde.hpp"


using namespace std;


int main() {

    double trendEur = 0.3;
    double volEur = 0.20;

    ChangeZC *ZCEurGBP = new ChangeZC("EUR/GBP","EUR/GBP",Change::EUR,Change::GBP,trendEur, SPOT_GBP, volEur);
    assert(ZCEurGBP != NULL);

    ZCEurGBP->Print();


    return 0;
}