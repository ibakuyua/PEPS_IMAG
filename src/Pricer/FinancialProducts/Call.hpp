#ifndef PEPS_IMAG_CALL_HPP
#define PEPS_IMAG_CALL_HPP


#include "ProductGen.hpp"

class Call : public ProductGen {

public:


    Call(PricerGen *pricer, Asset **assets, int size, int hedgingDateNb, double strike);

    virtual ~Call();

    virtual void MAJPortfolio();
};


#endif //PEPS_IMAG_CALL_HPP
