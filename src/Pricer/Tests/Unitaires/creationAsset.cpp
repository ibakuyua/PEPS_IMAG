#include <iostream>
#include <assert.h>
#include "../../FinancialProducts/Asset/Asset.hpp"

using namespace std;


int main(){

    cout << "Instance of an Asset" << endl;
    double trendJP = 0.03;
    double spot = 100;
    double volatility = 0.15;
    Asset * asset = new Asset("JP Morgan","JP Morgan",Change::USD,trendJP,spot,volatility);
    assert(asset != NULL);

    asset->Print();

    return 0;
}