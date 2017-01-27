#ifndef PEPS_IMAG_PAYOFFFUNCTIONS_HPP
#define PEPS_IMAG_PAYOFFFUNCTIONS_HPP

#include "pnl/pnl_matrix.h"
#include "../../RateModels/RateModelGen.hpp"
#include <map>
/*
 * Prototypes for payOff functions
 * CONVENTION : double payOffProduct(PnlMat *path, PnlVect *parameters, map<Change,RateModelGen*> &rateModels))
 */

/**
 *  Pay off for the product multimonde21
 *
 *  Remarks : pay off with only index point (without changes)
 *
 *  CONVENTION :
 *  -parameter is not used
 *  -rateModels is not used
 *  -path[][0..5] contain the simulation of 6 indexes basket in this order
 */
double payOffMultimonde21_simple(PnlMat *path, PnlVect *parameters, map<Change,RateModelGen*> &rateModels);

/**
 * Pay off for the product multimonde21
 *
 * remarks : the pay off is a function of each asset in the domestic economy
 *
 * CONVENTION :
 * -parameter is not used
 * -rateModels contain the rate models for each economy (eur,gbp,usd,hkd,jpy,aud)
 * -path[][0] contain the index eurostock       - path[][1] contain the index Xftse in euro
 * -path[][2] contain the index Xp500 in euro   - path[][3] contain the index Xhangseng in euro
 * -path[][4] contain the index Xnikkei in euro - path[][5] contain the index Xspasx200 in euro
 * -path[][6] contain the ZC XRgbp in euro      - path[][7] contain the ZC XRusd in euro
 * -path[][8] contain the ZC XRcny in euro      - path[][9] contain the ZC XRjpy in euro
 * -path[][10] contain the ZC XRaud in euro
 */
double payOffMultimonde21(PnlMat *path, PnlVect *parameters, map<Change,RateModelGen*> &rateModels);

/**
 * Pay off for the classic Call
 *
 *  CONVENTION :
 *  -parameter[0] contain the strike
 *  -rateModels is not used
 *  -path[][0] contain the simulation of the underlying asset
 */
double payOffCall(PnlMat *path, PnlVect *parameters, map<Change,RateModelGen*> &rateModels);


#endif //PEPS_IMAG_PAYOFFFUNCTIONS_HPP
