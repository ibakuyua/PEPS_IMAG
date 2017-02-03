#ifndef PEPS_IMAG_PRODUCTGEN_HPP
#define PEPS_IMAG_PRODUCTGEN_HPP

#include "Asset/Asset.hpp"
#include "../Pricing/PricerGen.hpp"
#include "../FinancialProducts/PayOffs/PayOffFunctions.hpp"
#include "Asset/AssetList.hpp"

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
    AssetList *assets; /// List of underlying assets
    int hedgingDateNb; /// Number of hedging dates (must be > nbTimeStep date)

    /*
     * PayOff
     */
    PayOffFunction payOff; /// A payOff function
    PnlVect *parameters; /// Parameters for the payOff

    /**
     * Hedging Portfolio
     */
    PnlVect *composition; /// Composition of the portfolio with underlying assets and free risk asset at the last row
    PnlVect *icComposition; /// IC for underlying risk asset composition

    /**
     * Constructor/Destructor
     *
     * @param[in] nom : the name of the product
     * @param[in] pricer : a pricer for the product valuation
     * @param[in] size : the number of assets
     * @param[in] hedgingDateNb : the number of hedging dates
     * @param[in] payOff : the payOff function
     * @param[in] parameters : list of parameters for the payOff(optionnal)
     * @param[in] assets : list of underlying assets (optionnal but has to be setted before to price)
     */
    ProductGen(string nom, PricerGen *pricer, int hedgingDateNb, PayOffFunction payOff,
               AssetList *assets = NULL, PnlVect* parameters = NULL);
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
     * Print : permit to show the information of the product
     */
    virtual void Print() const;

    /**
     * MAJPortfolio : permit to upgrade the portfolio (new composition to hedge)
     *
     * @param[in] t : the date to update
     *
     */
    virtual void UpdatePortfolio(double t);
private:
    PnlMat *pastQuotes; /**< Quotations until t of the market */
    PnlVect *spotQuotes; /**< Quotations at t */
};


#endif //PEPS_IMAG_PRODUCTGEN_HPP
