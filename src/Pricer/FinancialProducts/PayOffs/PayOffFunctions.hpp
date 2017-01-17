#ifndef PEPS_IMAG_PAYOFFFUNCTIONS_HPP
#define PEPS_IMAG_PAYOFFFUNCTIONS_HPP

#include "pnl/pnl_matrix.h"
/*
 * Prototypes for payOff functions
 * CONVENTION : double payOffProduct(PnlMat *path, PnlVect *parameters)
 */

/*
 *  Pay off for the product multimonde21
 *
 *  Remarks : pay off with only index point (without changes)
 *
 *  CONVENTION :
 *  -parameter is null
 *  -path[][0..5] contain the simulation of 6 indexes basket
 */
double payOffMultimonde21_simple(PnlMat *path, PnlVect *parameters);

// TODO : payOff avec les sous-jacents du multimonde21 en devise européenne

/*
 * Pay off for the classic Call
 *
 *  CONVENTION :
 *  -parameter[0] contain the strike
 *  -path[][0] contain the simulation of the underlying asset
 */
double payOffCall(PnlMat *path, PnlVect *parameters);


#endif //PEPS_IMAG_PAYOFFFUNCTIONS_HPP
