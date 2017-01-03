#ifndef PRICING_MULTIMONDE_HPP
#define PRICING_MULTIMONDE_HPP

#include "infoMultimonde.hpp"

#include "Pricing/PricerGen.hpp"

class Multimonde {
public:
    /**
     * Members
     */
    static constexpr double maturity = NB_DAYS_TO_MATURITY/(double)BASE;
    PnlVect *composition;
    PricerGen *pricer;

    /**
     * Constructor
     */
    Multimonde(PricerGen *price);

    /**
     * Methods
     */
    void PricePortfolio(double t, double &price);
    void PriceMultimonde(double t, double &price, double &ic);
    void MAJPortfolio();
    /**
     * PayOff
     */
    static double payOff(PnlMat *path);

};


#endif //PRICING_MULTIMONDE_HPP
