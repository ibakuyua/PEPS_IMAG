#include <pnl/pnl_matrix.h>
#include "AssetList.hpp"


AssetList::AssetList(int size, Asset **assets, PnlMat *correl, bool withDestroy)
        : size(size), assets(assets), correlMatrix(correl), hasToDestroy(withDestroy)
{
}

AssetList::~AssetList() {
    if (hasToDestroy){
        free(assets);
        pnl_mat_free(&correlMatrix);
    }

}

AssetList::AssetList(Asset *asset) {
    hasToDestroy = true;
    size = 1;
    assets = (Asset **) malloc(1 * sizeof(Asset *));
    assets[0] = asset;
    correlMatrix = pnl_mat_create_from_scalar(1,1,1.);
}

void AssetList::Print() {
    cout << "\nNumber of underlying assets : " << this->size << "\n";
    for (int d = 0; d < size; ++d) {
        cout << "\n";
        assets[d]->Print();
    }
    cout << "\n\nCorrelation matrix : \n";
    pnl_mat_print(correlMatrix);
}
