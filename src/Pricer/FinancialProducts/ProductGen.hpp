#ifndef PEPS_IMAG_PRODUCTGEN_HPP
#define PEPS_IMAG_PRODUCTGEN_HPP

#include "Asset.hpp"
#include "../Pricing/PricerGen.hpp"
#include "../PayOffs/PayOffFunctions.hpp"

/**
 * Abstract class for a financial product
 */
class ProductGen {
public:
    /*
     * Members
     */
    string nom; // Name of the product
    PricerGen *pricer; // Pricer for this product
    Asset **assets; // List of underlying assets
    int size; // Number of underlying assets
    int hedgingDateNb; // Number of hedging dates

    /*
     * PayOff
     */
    PayOffFunction payOff; //A payOff function
    PnlVect *parameters; // Parameters for the payOff

    /**
     * Hedging Portfolio
     */
    PnlVect *composition; // Composition of the portfolio with underlying assets

    /*
     * Constructor/Destructor
     */
    ProductGen(string nom, PricerGen *pricer, Asset **assets, int size, int hedgingDateNb, PayOffFunction payOff);
    virtual ~ProductGen();

    /*
     * Virtual method
     */
    virtual void PricePortfolio(double t, double &price) const;
    virtual void PriceProduct(double t, double &price, double &ic) const;

    /*
     * Virtual pur method
     */
    virtual void MAJPortfolio() = 0;

};


#endif //PEPS_IMAG_PRODUCTGEN_HPP
