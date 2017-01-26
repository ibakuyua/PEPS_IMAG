#ifndef PEPS_IMAG_CHANGEZC_HPP
#define PEPS_IMAG_CHANGEZC_HPP

#include "Asset.hpp"

/**
 * ChangeZC
 *
 * Represent the foreignZC in domestic value (X*Rf)
 *
 * remarks : the difference is that the trend is trend + r_f(t)
 */
class ChangeZC : public Asset{
public:
    /**
     * Members
     */
    Change foreignChange;

    /**
     * Constructor/Destructor
     */
    ChangeZC(string id, string name, Change domesticChange, Change foreignChange,
             double trend,double spot , double volatility);

};


#endif //PEPS_IMAG_CHANGEZC_HPP
