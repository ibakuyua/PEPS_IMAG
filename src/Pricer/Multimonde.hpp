#ifndef PRICING_MULTIMONDE_HPP
#define PRICING_MULTIMONDE_HPP


#include "Pricing/PricerGen.hpp"

class Multimonde {
public:
    /**
     * Members
     */
    PnlVect *composition;
    PricerGen *pricer;

    /**
     * Constructor
     */
    Multimonde(PricerGen *pricer);

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
