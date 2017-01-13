#ifndef PRICING_MULTIMONDE_HPP
#define PRICING_MULTIMONDE_HPP

#include "../infoMultimonde.hpp"
#include "ProductGen.hpp"

class Multimonde : public ProductGen {
public:
    /**
     * Members
     */
    static constexpr double maturity = NB_DAYS_TO_MATURITY/(double)BASE;

    /**
     * Constructor
     */
    Multimonde(PricerGen *price, Asset **assets, int size);

    /**
     * Methods
     */
    void PricePortfolio(double t, double &price);
    void PriceProduct(double t, double &price, double &ic);
    void MAJPortfolio();
    /**
     * PayOff
     */
    static double payOff(PnlMat *path);

};


#endif //PRICING_MULTIMONDE_HPP
