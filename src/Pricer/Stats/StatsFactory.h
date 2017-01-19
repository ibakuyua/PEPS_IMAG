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
    // TODO mettre tout en static

public:


    PnlMat *quotes_;
    PnlMat *returns_;
    PnlMat *covar_;
    PnlMat *correl_;
    PnlVect *mean_;
    PnlVect *vol_;
    PnlMat *quotesInEUR_;


    StatsFactory(PnlMat *quotes);
    ~StatsFactory();

private:

    //Compute returns from quotes
    void getQuotesInEUR();

    void getReturnsFromQuotes();

    void getMeanFromReturns();

    void getCovarFromReturns();

    void getVolFromCovar();

    void getCorrelFromCovar();




};


#endif //PEPS_IMAG_STATSFACTORY_H
