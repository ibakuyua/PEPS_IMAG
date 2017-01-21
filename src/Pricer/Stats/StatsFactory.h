#ifndef PEPS_IMAG_STATSFACTORY_H
#define PEPS_IMAG_STATSFACTORY_H

#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_array.h"
#include <cmath>

/**
 * StatsFactory
 *
 * Factory to statistics functions
 */
class StatsFactory {
public:


    PnlMat *quotes_; /// Matrix with time in row and asset in column
    PnlMat *logReturns_; /// Log return between t and t+1 for each asset
    PnlMat *covar_; /// Covar matrix for the log return
    PnlMat *correl_; /// Correlation matrix for the log return
    PnlVect *mean_; /// Mean for each log return asset
    PnlVect *vol_; /// Volatility for each log return asset


    /**
     * Constructor/Destructor
     *
     * @param[in] quotes : the quotes of the underlying assets
     */
    StatsFactory(PnlMat *quotes);
    ~StatsFactory();

private:
    void ComputeLogReturnsFromQuotes();

    void ComputeMeanFromLogReturns();

    void ComputeCovarFromLogReturns();

    void ComputeVolFromCovar();

    void ComputeCorrelFromCovar();




};


#endif //PEPS_IMAG_STATSFACTORY_H
