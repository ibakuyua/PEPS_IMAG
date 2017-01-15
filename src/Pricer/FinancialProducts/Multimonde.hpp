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
     * Constructor (the payOff is initialized in the cpp)
     */
    Multimonde(PricerGen *price, Asset **assets, int size);

    /**
     * Overriding Methods
     */
    void MAJPortfolio();
};


#endif //PRICING_MULTIMONDE_HPP
