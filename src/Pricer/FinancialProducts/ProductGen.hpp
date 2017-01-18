#ifndef PEPS_IMAG_PRODUCTGEN_HPP
#define PEPS_IMAG_PRODUCTGEN_HPP

#include "Asset.hpp"
#include "../Pricing/PricerGen.hpp"
#include "../FinancialProducts/PayOffs/PayOffFunctions.hpp"

/**
 * ProductGen[Abstract]
 *
 * Class for a financial product
 */
class ProductGen {
public:
    /*
     * Members
     */
    string nom; /// Name of the product
    PricerGen *pricer; /// Pricer for this product
    Asset **assets; /// List of underlying assets
    int size; /// Number of underlying assets
    int hedgingDateNb; /// Number of hedging dates

    /*
     * PayOff
     */
    PayOffFunction payOff; /// A payOff function
    PnlVect *parameters; /// Parameters for the payOff

    /**
     * Hedging Portfolio
     */
    PnlVect *composition; /// Composition of the portfolio with underlying assets

    /*
     * Constructor/Destructor
     *
     * @param[in] nom : the name of the product
     * @param[in] pricer : a pricer for the product valuation
     * @param[in] assets : list of underlying assets
     * @param[in] size : the number of assets
     * @param[in] hedgingDateNb : the number of hedging dates
     * @param[in] payOff : the payOff function
     */
    ProductGen(string nom, PricerGen *pricer, Asset **assets, int size, int hedgingDateNb, PayOffFunction payOff);
    virtual ~ProductGen();

    /*
     * Virtual methods
     */
    /**
     * PricePortfolio : permit to price the hedging portfolio
     *
     * remarks : composition . prixAsset(t)
     *
     * @param[in] t : the date to get the asset prices in the market
     * @param[out] price : the Price of the hedging portfolio
     */
    virtual void PricePortfolio(double t, double &price) const;
    /**
     * PriceProduct : permit to price the product thanks to the pricer
     *
     * @param[in] t : the date to price the product
     * @param[out] Price : the result of the product valuation
     * @param[out] ic : width of the confidence interval
     */
    virtual void PriceProduct(double t, double &price, double &ic) const;
    /**
     * PrintProduct : permit to show the information of the product
     */
    virtual void PrintProduct() const;

    /*
     * Virtual pur method
     */
    /**
     * MAJPortfolio : permit to upgrade the portfolio (new composition to hedge)
     */
    virtual void MAJPortfolio() = 0;

};


#endif //PEPS_IMAG_PRODUCTGEN_HPP
