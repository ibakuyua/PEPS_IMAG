//
// Created by Ruimy Benjamin Mac on 13/01/2017.
//

#ifndef PEPS_IMAG_STATSFACTORY_H
#define PEPS_IMAG_STATSFACTORY_H

#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_array.h"
#include <cmath>

class StatsFactory {

public:


    PnlMat *quotes_;


    PnlMat *returns_;
    PnlMat *covar_;
    PnlMat *correl_;
    PnlVect *mean_;
    PnlVect *vol_;


    StatsFactory(PnlMat *quotes);
    ~StatsFactory();

private:

    //Compute returns from quotes
    void getReturnsFromQuotes();

    void getMeanFromReturns();

    void getCovarFromReturns();

    void getVolFromCovar();

    void getCorrelFromCovar();




};


#endif //PEPS_IMAG_STATSFACTORY_H
