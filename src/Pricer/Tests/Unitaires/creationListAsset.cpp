#include <iostream>
#include <assert.h>
#include "../../FinancialProducts/Asset/Asset.hpp"
#include "../../FinancialProducts/Asset/AssetList.hpp"


using namespace std;


int main(){

    cout << "Instance of Assets" << endl;

    //JP Morgan Asset
    double trendJP = 0.03;
    double spotJP = 100;
    double volatilityJP = 0.15;

    //AXA Asset
    double trendAXA = 0.03;
    double spotAXA = 100;
    double volatilityAXA = 0.15;


    Asset **myAssets = (Asset**) malloc(2 * sizeof(Asset*));
    myAssets[0] = new Asset("JP Morgan","JP Morgan",Change::USD,trendJP,spotJP,volatilityJP);
    myAssets[1] = new Asset("AXA","AXA",Change::EUR,trendAXA,spotAXA,volatilityAXA);

    PnlMat *correlationMatrixEur = NULL;

    AssetList *assetList = new AssetList(2, myAssets, correlationMatrixEur, true);


    cout << "Voici la liste des asset : \n";
    assetList->Print();
    //Erreur de seg car il faut une mattrice de correl (pas de ptr null)


    return 0;
}