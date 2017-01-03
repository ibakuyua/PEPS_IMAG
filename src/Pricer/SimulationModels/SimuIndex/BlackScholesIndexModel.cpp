#include "BlackScholesIndexModel.hpp"


BlackScholesIndexModel::BlackScholesIndexModel(int assetNb, PnlVect *spot, PnlVect *trend, PnlVect *volatility,
                                               PnlMat *choleskyCorr, RateModelGen **rateModel)
        : ModelGen(assetNb, spot, trend, volatility, rateModel){
    this->choleskyCorr = choleskyCorr;
    this->Gi_ = pnl_vect_new();
    this->LGi_ = pnl_vect_new();
    this->St = pnl_vect_new();
}

void BlackScholesIndexModel::Simulate(double t, double maturity, PnlMat *path, const PnlMat *past, int stepNb) {
// Simple case of t == 0
    if (t==0. || past == NULL) {
        Simulate(maturity, path, stepNb);
        return;
    }
// Case in t != 0
    // Copy of the past matrix in the path matrix before S_t
    for (int i = 0; i < past->m - 1; ++i) {
        for (int d = 0; d < past->n; ++d) {
            PNL_MSET(path,i,d,MGET(past,i,d));
        }
    }
    // NB : The last row is S(t), doesn't belong to a step of constatation !
    pnl_vect_resize(St,path->n);
    pnl_mat_get_row(St, past, past->m - 1);
    // Size of a step
    double step = maturity/(double)stepNb;
    double sqrtStep = sqrt(step);
    // The index after t
    int indexAftert = past->m - 1;
    // Number of value to simulate
    int nbToSimulate = stepNb - indexAftert + 1;
    // Initialisation of previous value due to simulate S~
    double value_tiMinus1[path->n];
    // Size of the first step (t_(i+1) - t)
    double firstStep = MAX(step * indexAftert - t,0);
    double sqrtFirstStep = sqrt(firstStep);
    // G_0 : First gaussian vector
    pnl_vect_rng_normal_d(Gi_,path->n,rng);
    // All the LdGi
    LGi_ = pnl_mat_mult_vect(choleskyCorr,Gi_);

    double sigma_d, Sd_t, LdGi, value;
    for (int d = 0; d < path->n; ++d) {
        sigma_d = GET(volatility,d);
        Sd_t = GET(St, d);
        LdGi = GET(LGi_, d);

        value = exp(rateModels[d]->GetIntegralRate(t, t + firstStep) - (sigma_d * sigma_d / 2) * firstStep + sigma_d * sqrtFirstStep * LdGi);

        // Store in path
        MLET(path, indexAftert, d) = (Sd_t * value);
        // Store in value_tiMinus1
        value_tiMinus1[d] = value;
    }
    // For all other simulation
    double tiMinus1;
    for (int i = 1; i < nbToSimulate; ++i) {
        // Same processus that beside but with a different step
        tiMinus1 = (indexAftert + i - 1) * step;
        pnl_vect_rng_normal_d(Gi_, path->n, rng);
        LGi_ = pnl_mat_mult_vect(choleskyCorr, Gi_);
        // For each asset
        for (int d = 0; d < path->n; ++d) {
            sigma_d = GET(volatility,d);
            Sd_t = GET(St, d);
            LdGi = GET(LGi_, d);

            value = value_tiMinus1[d] * exp(rateModels[d]->GetIntegralRate(tiMinus1, tiMinus1 + step)
                                            - (sigma_d * sigma_d/2) * step + sigma_d * sqrtStep * LdGi);

            // Store in path and value_tiMinus1
            PNL_MSET(path, (indexAftert + i), d, (Sd_t * value));
            value_tiMinus1[d] = value;
        }
    }
}


void BlackScholesIndexModel::Simulate(double maturity, PnlMat *path, int stepNb) {
    // Step initialisation
    double step = maturity/(double)stepNb;
    double sqrtStep = sqrt(step);
    // Spot initialisation
    for (int d = 0; d < path->n; ++d)
        PNL_MSET(path, 0, d, GET(spot,d));
    double sigma_d, Sd_tiMinus1, LdGi, tiMinus1, Sd_ti;
    // For each time
    for (int i = 1; i < path->m; ++i) {
        tiMinus1 = (i-1) * step;
        pnl_vect_rng_normal_d(Gi_,path->n,rng); // Gi gaussian vector
        LGi_ =  pnl_mat_mult_vect(choleskyCorr,Gi_); // All the LdGi
        // For each asset
        for (int d = 0; d < path->n; ++d) {
            sigma_d = GET(volatility,d);
            Sd_tiMinus1 = PNL_MGET(path, (i-1), d);
            LdGi = GET(LGi_,d);

            Sd_ti = Sd_tiMinus1 * exp(rateModels[d]->GetIntegralRate(tiMinus1, tiMinus1 + step)
                                      - (sigma_d * sigma_d / 2) * step + sigma_d * sqrtStep * LdGi);

            PNL_MSET(path,i,d,Sd_ti);
        }
    }
}

BlackScholesIndexModel::~BlackScholesIndexModel() {
    pnl_vect_free(&Gi_);
    pnl_vect_free(&LGi_);
    pnl_vect_free(&St);
    // Base destructor is called here
}
