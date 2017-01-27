#include <time.h>
#include "ModelGen.hpp"
#include "../FinancialProducts/Asset/ChangeZC.hpp"


ModelGen::ModelGen(int assetNb, int economyNb, RateModelGen **rateModels, string name)
        :assetNb(assetNb), name(name)
{
    spot = pnl_vect_create_from_scalar(assetNb,0);
    trend = pnl_vect_create_from_scalar(assetNb,0);
    volatility = pnl_vect_create_from_scalar(assetNb,0);
    for (int e = 0; e < economyNb; ++e)
        this->rateModels[rateModels[e]->change] = rateModels[e];

    rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, time(NULL));
}

void ModelGen::ShiftAsset(PnlMat *path_shifted, const PnlMat *path, int d, double h, double t, double timestep) {
    // Index i after t
    int indexAfter_t = (t / timestep - (int)(t / timestep) < 0.000000001)
                       ? (int)(t/timestep)
                       : (int)(t/timestep) + 1;
    //int indexAfter_t = (int)(t / timestep) + 1;
    // path_shifted is path with asset d shifted after t

    pnl_mat_clone(path_shifted, path);
    
    for (int i = indexAfter_t; i < path->m; ++i) {
        if (i != 0)
            MLET(path_shifted,i,d) *= (1+h);

    }
}

void ModelGen::SetAssets(AssetList *assets) {
    if (assets->size != assetNb)
        throw domain_error("SetAssets : list asset doesn't contain "
                                   "the same number of asset that mentionned in the assetNb model");
    for (int d = 0; d < assets->size; ++d) {
        LET(spot,d) = assets->assets[d]->spot;
        LET(trend,d) = assets->assets[d]->trend;
        LET(volatility,d) = assets->assets[d]->volatility;
    }
    this->assetList = assets;
}

void ModelGen::Print() {
    cout << this->name;
}

ModelGen::~ModelGen() {
    pnl_vect_free(&spot);
    pnl_vect_free(&trend);
    pnl_vect_free(&volatility);
    pnl_rng_free(&rng);
}
