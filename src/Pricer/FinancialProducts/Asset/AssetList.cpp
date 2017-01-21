#include <pnl/pnl_matrix.h>
#include "AssetList.hpp"


AssetList::AssetList(int size, Asset **assets, PnlMat *vol, bool withDestroy)
        : size(size), assets(assets), volatilityMat(vol), hasToDestroy(withDestroy)
{
}

AssetList::~AssetList() {
    if (hasToDestroy){
        free(assets);
        pnl_mat_free(&volatilityMat);
    }

}

AssetList::AssetList(Asset *asset) {
    hasToDestroy = true;
    size = 1;
    assets = (Asset **) malloc(1 * sizeof(Asset *));
    assets[0] = asset;
    volatilityMat = pnl_mat_create_from_scalar(1,1,1.);
}

void AssetList::Print() {
    cout << "\nNumber of underlying assets : " << this->size;
    for (int d = 0; d < size; ++d) {
        cout << "\n";
        assets[d]->Print();
    }
}
