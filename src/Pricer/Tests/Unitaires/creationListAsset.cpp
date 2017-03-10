#include <iostream>
#include <assert.h>
#include <pnl/pnl_matrix.h>
#include "../../FinancialProducts/Asset/Asset.hpp"
#include "../../FinancialProducts/Asset/AssetList.hpp"


using namespace std;


int main(){

    cout << "Instance of Assets" << endl;

    //JP Morgan Asset
    double trendJP = 0.04;
    double spotJP = 46.53;
    double volatilityJP = 0.20;

    //AXA Asset
    double trendAXA = 0.03;
    double spotAXA = 165.53;
    double volatilityAXA = 0.15;


    Asset **myAssets = (Asset**) malloc(2 * sizeof(Asset*));
    myAssets[0] = new Asset("JP Morgan","JP Morgan",Change::USD,trendJP,spotJP,volatilityJP);
    myAssets[1] = new Asset("AXA","AXA",Change::EUR,trendAXA,spotAXA,volatilityAXA);

    PnlMat *correlationMatrixEur = pnl_mat_new();

    AssetList *assetList = new AssetList(2, myAssets, correlationMatrixEur, true);
    assert(assetList != NULL);

    cout << "Voici la liste des asset : \n";
    assetList->Print();

    return 0;
}