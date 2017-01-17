#ifndef PRICING_MARCHE_HPP
#define PRICING_MARCHE_HPP

#include "FinancialProducts/ProductGen.hpp"

/*
 * CotationTypes
 *
 * Enum which represent the type of market cotations
 */
typedef enum {
    Real = 0,
    Historical = 1,
    Simulated = 2
}CotationTypes;

/**
 * Marche
 *
 * Represent the market with the product
 *
 * Singleton class
 */
class Marche {
public:
    static double t; /// Current time
    PnlMat *cours; /// The cotations in the market (0 to T or 0 to t if real)
    ProductGen *product; /// The product to valuate

    /**
     * Instance : return an instance of the market
     *
     * remarks : permit to instance the market just one time
     *
     * @param[in] product : the product
     * @return a market
     */
    static Marche *Instance(ProductGen *product);

    /**
     * ImportCotations : permit to fill the cotation market
     *
     * @param[in] type : type of cotations
     */
    void ImportCotations(CotationTypes type);

    /**
     * Permit to obtain the cotations for the date t
     *
     * remarks : exception if t > current date of the market and cotation type is real
     *
     * @param[in] t : the date to get the cotations
     * @param[out] cotations : the result
     */
    static void GetCotations(double t, PnlVect* cotations);


private:
    static Marche *instance; /// The unique instance of market
    static PnlVect *cours_t; /// The cotations at the date t
    CotationTypes type; /// The type of current cotations in the market
    /**
     * Constructor/Destructor
     */
    Marche(ProductGen *product);
    ~Marche();
};


#endif //PRICING_MARCHE_HPP
