#include <time.h>
#include "ModelGen.hpp"
#include "../FinancialProducts/Asset/ChangeZC.hpp"


ModelGen::ModelGen(int assetNb, int economyNb, RateModelGen **rateModels, string name)
        :assetNb(assetNb), name(name)
{
    spot = pnl_vect_create_from_scalar(assetNb,0);
    trend = pnl_vect_create_from_scalar(assetNb,0);
    volatility = pnl_vect_create_from_scalar(assetNb,0);
    associatedChanges = vector<Change>(assetNb);
    for (int i = 0; i < assetNb; ++i)
        associatedChanges[i] = Change::EUR;
    for (int e = 0; e < economyNb; ++e)
        this->rateModels[rateModels[e]->change] = rateModels[e];

    rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, time(NULL));
}

void ModelGen::SetAssets(AssetList *assets) {
    for (int d = 0; d < assets->size; ++d) {
        LET(spot,d) = assets->assets[d]->spot;
        LET(trend,d) = assets->assets[d]->trend;
        LET(volatility,d) = assets->assets[d]->volatility;
        associatedChanges[d] = assets->assets[d]->change;
    }
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
