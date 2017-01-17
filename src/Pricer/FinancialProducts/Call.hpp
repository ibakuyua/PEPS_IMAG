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
     * @param[in] assets : the list of assets (contain just one asset)
     * @param[in] size : the number of underlying assets (contain 1)
     * @param[in] hedgingDateNb : the number of date of modification of the hedging portfolio
     * @param[in] strike : the strike of the call
     */
    Call(PricerGen *pricer, Asset **assets, int size, int hedgingDateNb, double strike);
    virtual ~Call();

    /**
     * Implementation of virtual pure method
     */
    virtual void MAJPortfolio();
};


#endif //PEPS_IMAG_CALL_HPP
