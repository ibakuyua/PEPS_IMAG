#ifndef PRICING_MARCHE_HPP
#define PRICING_MARCHE_HPP

#include "FinancialProducts/ProductGen.hpp"

typedef enum {
    Real = 0,
    Historical = 1,
    Simulated = 2
}CotationTypes;

/**
 * Represent the market with the product
 *
 * Singleton class
 */
class Marche {
public:
    static double t;
    PnlMat *cours;
    static PnlVect *cours_t;
    ProductGen *product;

    static Marche *Instance(ProductGen *product);

    void ImportCotations(CotationTypes type);
    static PnlVect *GetCotations(double t);


private:
    static Marche *instance;
    Marche(ProductGen *product);
    ~Marche();
};


#endif //PRICING_MARCHE_HPP
