#ifndef PEPS_IMAG_PAYOFFFUNCTIONS_HPP
#define PEPS_IMAG_PAYOFFFUNCTIONS_HPP

#include "pnl/pnl_matrix.h"
/*
 * Prototypes for payOff functions
 * convention double payOffProduct(PnlMat *path, PnlVect *parameters)
 */

/*
 *  Pay off for the product multimonde21
 *
 *  CONVENTION :
 *  //TODO
 */
double payOffMultimonde21(PnlMat *path, PnlVect *parameters);

// Pay off for the call
double payOffCall(PnlMat *path, PnlVect *parameters);


#endif //PEPS_IMAG_PAYOFFFUNCTIONS_HPP
