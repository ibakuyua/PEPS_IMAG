#include <time.h>
#include "ModelGen.hpp"


ModelGen::ModelGen(int assetNb, PnlVect *spot, PnlVect *trend, PnlVect *volatility, RateModelGen *rateModel)
        : assetNb(assetNb), spot(spot), trend(trend), volatility(volatility), rateModel(rateModel)
{
    rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, time(NULL));
}


ModelGen::~ModelGen() {
    pnl_rng_free(&rng);
}
