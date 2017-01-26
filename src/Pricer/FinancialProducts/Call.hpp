#ifndef PEPS_IMAG_CALL_HPP
#define PEPS_IMAG_CALL_HPP


#include "ProductGen.hpp"

/**
 * Call
 *
 * Represent the vanilla call
 */
class Call : public ProductGen {
public:

    /**
     * Constructor/Destructor
     *
     * @param[in] pricer : the pricer to valuate the call
     * @param[in] assets : the underlying asset
     * @param[in] hedgingDateNb : the number of date of modification of the hedging portfolio
     * @param[in] strike : the strike of the call
     */
    Call(PricerGen *pricer, Asset * asset, int hedgingDateNb, double strike);
    virtual ~Call();

    /**
     * Virtual methods overrided
     */
    void Print() const;
};


#endif //PEPS_IMAG_CALL_HPP
