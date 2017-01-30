#include "BlackScholesModel.hpp"


BlackScholesModel::BlackScholesModel(int assetNb, int economyNb, RateModelGen **rateModel)
        : ModelGen(assetNb, economyNb, rateModel, "Black Scholes"){
    this->choleskyCorr = pnl_mat_create_from_scalar(assetNb,assetNb,0);
    for (int i = 0; i < assetNb; ++i)
        MLET(choleskyCorr,i,i) = 1.;
    this->Gi_ = pnl_vect_new();
    this->LGi_ = pnl_vect_new();
    this->St = pnl_vect_new();
    this->valuet_iminus1 = pnl_vect_new();
}

void BlackScholesModel::Simulate(double t, double maturity, PnlMat *path, const PnlMat *past, int stepNb, PnlVect *simulationSchedule) {

    // Simple case in t = 0.
    if (t==0. || past == NULL){
        Simulate(maturity,path, stepNb, simulationSchedule);
        return;
    }
    // Case in t != 0.
    // Resize
    pnl_mat_resize(path,stepNb + 1,assetNb);
    pnl_vect_resize(valuet_iminus1, path->n);
    // Copy the past matrix in path before S_t
    for (int i = 0; i < past->m - 1; ++i)
        for (int d = 0; d < path->n; ++d) // Get only the choosed number of asset (maybe past can have more asset)
            PNL_MSET(path,i,d,MGET(past,i,d));
    // NB : The last row is S(t), doesn't belong to a step of constatation !
    pnl_mat_get_row(St, past, past->m - 1);
    // Resize in the case where past contains more asset than path
    pnl_vect_resize(St,path->n);
    // Compute the index after t and the first step
    int indexAftert = past->m - 1;
    double cumuledStep = 0.;
    double step = 0., firstStep = 0., sqrtFirstStep = 0.;
    if (simulationSchedule == NULL){
        step = maturity / (double)stepNb;
        firstStep = MAX(step * indexAftert - t, 0.);
    }else {
        for (int i = 0; i < indexAftert; ++i) {
            if (i >= simulationSchedule->size) {
                cumuledStep = maturity;
                break;
            }
            cumuledStep += GET(simulationSchedule, i);
        }
        firstStep = MAX(cumuledStep - t, 0.);
    }
    sqrtFirstStep = sqrt(firstStep);
    // If we are situated in a constatation date
    bool isInConstationDate = (firstStep <= 0.0000001);
    // First simulation
    double sigma_d, Sd_t, LdGi, value;
    if (isInConstationDate){
        // Add the spot to path
        pnl_mat_set_row(path,St,indexAftert);
        // Set S_timinus1 at 1.
        pnl_vect_set_all(valuet_iminus1,1.);
    }else{
        // G_0 First gaussian vector
        pnl_vect_rng_normal_d(Gi_,path->n,rng);
        // All the LdGi
        LGi_ = pnl_mat_mult_vect(choleskyCorr,Gi_);
        // For each asset
        for (int d = 0; d < path->n; ++d) {
            sigma_d = GET(volatility,d);
            Sd_t = GET(St,d);
            LdGi = GET(LGi_, d);

            value = exp(rateModels[assetList->assets[d]->change]->GetIntegralRate(t, t + firstStep)
                        - (sigma_d * sigma_d / 2) * firstStep + sigma_d * sqrtFirstStep * LdGi
                        );
            // Store in path
            MLET(path, indexAftert, d) = Sd_t * value;
            // Store in S_timinus1
            LET(valuet_iminus1,d) = value;
        }
    }
    // For all other simulations
    // Compute ti-1
    double tiMinus1 = (simulationSchedule == NULL)
                      ? indexAftert * step
                      : cumuledStep;
    double ti = tiMinus1;
    for (int i = indexAftert + 1; i < path->m; ++i) {
        // Compute ti
        ti += (simulationSchedule == NULL)
            ? step
            : GET(simulationSchedule,i-1);
        // Same process that besides but with different step
        pnl_vect_rng_normal_d(Gi_,path->n, rng);
        LGi_ = pnl_mat_mult_vect(choleskyCorr, Gi_);
        for (int d = 0; d < path->n; ++d) {
            sigma_d = GET(volatility,d);
            Sd_t = GET(St, d);
            LdGi = GET(LGi_, d);

            value = GET(valuet_iminus1,d) * exp(
                    rateModels[assetList->assets[d]->change]->GetIntegralRate(tiMinus1,ti)
                    - (sigma_d * sigma_d / 2.) * (ti - tiMinus1) + sigma_d * sqrt(ti - tiMinus1) * LdGi
            );
            // Store computed values
            PNL_MSET(path, i, d, Sd_t * value);
            LET(valuet_iminus1,d) = value;
        }
        // Maj ti-1
        tiMinus1 = ti;
    }
/*
    if(simulationSchedule == NULL){
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
        int indexBeforet = past->m - 1;
        // Number of value to simulate
        int nbToSimulate = stepNb - indexBeforet + 1;
        // Initialisation of previous value due to simulate S~
        double value_tiMinus1[path->n];
        // Size of the first step (t_(i+1) - t)
        double firstStep = MAX(step * indexBeforet - t,0);
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

            value = exp(rateModels[assetList->assets[d]->change]->GetIntegralRate(t, t + firstStep)
                        - (sigma_d * sigma_d / 2) * firstStep + sigma_d * sqrtFirstStep * LdGi
            );

            // Store in path
            MLET(path, indexBeforet, d) = (Sd_t * value);
            // Store in value_tiMinus1
            value_tiMinus1[d] = value;
        }
        // For all other simulation
        double tiMinus1;
        for (int i = 1; i < nbToSimulate; ++i) {
            // Same processus that beside but with a different step
            tiMinus1 = (indexBeforet + i - 1) * step;
            pnl_vect_rng_normal_d(Gi_, path->n, rng);
            LGi_ = pnl_mat_mult_vect(choleskyCorr, Gi_);
            // For each asset
            for (int d = 0; d < path->n; ++d) {
                sigma_d = GET(volatility,d);
                Sd_t = GET(St, d);
                LdGi = GET(LGi_, d);

                value = value_tiMinus1[d] * exp(
                        rateModels[assetList->assets[d]->change]->GetIntegralRate(tiMinus1, tiMinus1 + step)
                        - (sigma_d * sigma_d/2) * step + sigma_d * sqrtStep * LdGi
                );

                // Store in path and value_tiMinus1
                PNL_MSET(path, (indexBeforet + i), d, (Sd_t * value));
                value_tiMinus1[d] = value;
            }
        }
    }else{

        //Init historical data
        if(past == NULL){
            for (int d = 0; d < path->n; ++d)
                PNL_MSET(path, 0, d, GET(spot,d));
        }else{
            for (int i = 0; i < past->m - 1; ++i) {
                for (int d = 0; d < past->n; ++d) {
                    PNL_MSET(path,i,d,MGET(past,i,d));
                }
            }
        }
        // NB : The last row is S(t), doesn't belong to a step of constatation !
        pnl_vect_resize(St,path->n);
        pnl_mat_get_row(St, past, past->m - 1);

        // Size of a step
       //--> FAUX double step = GET(scheduleSimulation,0);


        double step;
        // The index before
        int indexBeforet;
        int tmp = 0;
        double firstStep;
        for(int i = 0; i < (simulationSchedule->size);i++){
            if(t <= tmp){
                indexBeforet = i;
                firstStep = MAX(tmp - t,0);
                break;
            }
            tmp+=GET(simulationSchedule,i);
        }

        double sqrtFirstStep = sqrt(firstStep);




        // Number of value to simulate
        int nbToSimulate = stepNb - indexBeforet ;

        // Initialisation of previous value due to simulate S~
        double value_tiMinus1[path->n];

        // Size of the first step (t_(i+1) - t)


        firstStep = MAX(step * indexBeforet - t,0);
        sqrtFirstStep = sqrt(firstStep);
        // G_0 : First gaussian vector
        pnl_vect_rng_normal_d(Gi_,path->n,rng);
        // All the LdGi
        LGi_ = pnl_mat_mult_vect(choleskyCorr,Gi_);

        double sigma_d, Sd_t, LdGi, value;
        for (int d = 0; d < path->n; ++d) {
            sigma_d = GET(volatility,d);
            Sd_t = GET(St, d);
            LdGi = GET(LGi_, d);

            value = exp(rateModels[assetList->assets[d]->change]->GetIntegralRate(t, t + firstStep)
                        - (sigma_d * sigma_d / 2) * firstStep + sigma_d * sqrtFirstStep * LdGi
            );

            // Store in path
            MLET(path, indexBeforet, d) = (Sd_t * value);
            // Store in value_tiMinus1
            value_tiMinus1[d] = value;
        }

        // For all other simulation
        double tiMinus1;
        for (int i = 1; i < nbToSimulate; ++i) {
            step = GET(simulationSchedule,i);
            double sqrtStep = sqrt(step);
            // Same processus that beside but with a different step
            tiMinus1 = (indexBeforet + i - 1) * step;
            pnl_vect_rng_normal_d(Gi_, path->n, rng);
            LGi_ = pnl_mat_mult_vect(choleskyCorr, Gi_);
            // For each asset
            for (int d = 0; d < path->n; ++d) {
                sigma_d = GET(volatility, d);
                Sd_t = GET(St, d);
                LdGi = GET(LGi_, d);

                value = value_tiMinus1[d] * exp(
                        rateModels[assetList->assets[d]->change]->GetIntegralRate(tiMinus1, tiMinus1 + step)
                        - (sigma_d * sigma_d / 2) * step + sigma_d * sqrtStep * LdGi
                );

                // Store in path and value_tiMinus1
                PNL_MSET(path, (indexBeforet + i), d, (Sd_t * value));
                value_tiMinus1[d] = value;
            }
        }
    }*/
}


void BlackScholesModel::Simulate(double maturity, PnlMat *path, int stepNb, PnlVect *simulationSchedule) {
    // Resize
    pnl_mat_resize(path,stepNb + 1, assetNb);
    // Spot initialisation
    for (int d = 0; d < path->n; ++d)
        PNL_MSET(path, 0, d, GET(spot,d));
    double step = maturity / (double)stepNb;
    double sigma_d, Sd_tiMinus1, LdGi, Sd_ti;
    double tiMinus1 = 0.;
    double ti = tiMinus1;
    for (int i = 1; i < path->m; ++i) {
        ti += (simulationSchedule == NULL)
              ? step
              : GET(simulationSchedule,i-1);
        pnl_vect_rng_normal_d(Gi_,path->n,rng); // Gi gaussian vector
        LGi_ =  pnl_mat_mult_vect(choleskyCorr,Gi_); // All the LdGi
        // For each asset
        for (int d = 0; d < path->n; ++d) {
            sigma_d = GET(volatility,d);
            Sd_tiMinus1 = PNL_MGET(path, (i-1), d);
            LdGi = GET(LGi_,d);

            Sd_ti = Sd_tiMinus1 * exp(
                    rateModels[assetList->assets[d]->change]->GetIntegralRate(tiMinus1, ti)
                    - (sigma_d * sigma_d / 2) * (ti - tiMinus1) + sigma_d * sqrt(ti - tiMinus1) * LdGi
            );

            PNL_MSET(path,i,d,Sd_ti);
        }
        tiMinus1 = ti;
    }
}

BlackScholesModel::~BlackScholesModel() {
    pnl_vect_free(&Gi_);
    pnl_vect_free(&LGi_);
    pnl_vect_free(&St);
    pnl_mat_free(&choleskyCorr);
    // Base destructor is called here
}

void BlackScholesModel::SimulateMarket(double maturity, PnlMat *path, int stepNb, Change domesticChange) {
    // Resize
    pnl_mat_resize(path,stepNb + 1,assetNb + 1); // Add the domestic ZC
    // Step Initialisation
    double step = maturity/(double)stepNb;
    double sqrtStep = sqrt(step);
    double addTrendForZC, ti_minus1;
    // Spot Initialisation
    for (int d = 0; d < path->n - 1; ++d)
        PNL_MSET(path, 0, d, GET(spot,d));
    PNL_MSET(path,0,path->n-1, 1.);
    double sigma_d, trend_d, Sd_tiMinus1, LdGi, Sd_ti;
    // For each time
    for (int i = 1; i < path->m; ++i) {
        ti_minus1 = (i-1) * step;
        pnl_vect_rng_normal_d(Gi_,path->n-1,rng); // Gi gaussian vector
        LGi_ =  pnl_mat_mult_vect(choleskyCorr,Gi_); // All the LdGi
        // For each asset
        for (int d = 0; d < path->n - 1; ++d) {
            sigma_d = GET(volatility,d);
            trend_d = GET(trend,d);
            Sd_tiMinus1 = PNL_MGET(path, (i-1), d);
            LdGi = GET(LGi_,d);

            // Case of X.ZC : XR_f
            addTrendForZC = (assetList->assets[d]->isChange.first)
                            ? rateModels[assetList->assets[d]->isChange.second]->GetIntegralRate(ti_minus1,ti_minus1+step)
                            : 0.;

            Sd_ti = Sd_tiMinus1 * exp( (trend_d - sigma_d * sigma_d / 2) * step + addTrendForZC + sigma_d * sqrtStep * LdGi);

            PNL_MSET(path,i,d,Sd_ti);
        }
        // For the domestic free risk asset
        PNL_MSET(path,i,path->n-1,exp(rateModels[Change::EUR]->GetIntegralRate(0.,ti_minus1+step)));
    }
}


void BlackScholesModel::SetAssets(AssetList *assets) {
    ModelGen::SetAssets(assets);
    pnl_mat_free(&choleskyCorr);
    choleskyCorr = pnl_mat_copy(assets->correlMatrix);
    pnl_mat_chol(choleskyCorr);
}

void BlackScholesModel::GetParametersFromStats(StatsFactory *stats, PnlVect **trend, PnlMat **volMatrix) {
    *volMatrix = pnl_mat_copy(stats->covar_);
    pnl_mat_chol(*volMatrix); // sigma = cholesky(sigma.sigmaT) = cholesky(covar)
    *trend = pnl_vect_copy(stats->mean_); // drift = mean
    PnlVect *vol;
    vol = pnl_vect_create_from_zero((*volMatrix)->n);
    for (int i = 0; i < (*trend)->size; ++i) { // drift = trend - 1/2 sigma_i^2
        pnl_mat_get_col(vol,*volMatrix,i);
        LET(*trend,i) = GET(*trend,i) + 1/2 * pnl_vect_scalar_prod(vol,vol);
    }
    pnl_vect_free(&vol);
}
