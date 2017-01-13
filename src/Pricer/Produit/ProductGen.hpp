#ifndef PEPS_IMAG_PRODUCTGEN_HPP
#define PEPS_IMAG_PRODUCTGEN_HPP

#include "Asset.hpp"
#include "../Pricing/PricerGen.hpp"

/**
 * Abstract class for a financial product
 */
class ProductGen {
public:
    string nom; // Name of the product
    PricerGen *pricer; // Pricer for this product
    Asset **assets; // List of underlying assets
    int size; // Number of underlying assets
    int hedgingDateNb; // Number of hedging dates
    PnlVect *composition; // Composition of the portfolio

    ProductGen(PricerGen *pricer, Asset **assets, int size, int hedgingDateNb);
    ~ProductGen();

    virtual void PricePortfolio(double t, double &price) = 0;
    virtual void PriceProduct(double t, double &price, double &ic) = 0;
    virtual void MAJPortfolio() = 0;

};


#endif //PEPS_IMAG_PRODUCTGEN_HPP
