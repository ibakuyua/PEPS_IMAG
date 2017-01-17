#ifndef PRICING_MULTIMONDE_HPP
#define PRICING_MULTIMONDE_HPP

#include "../infoMultimonde.hpp"
#include "ProductGen.hpp"

/**
 * Multimonde
 *
 * Represent the product multimonde21
 */
class Multimonde : public ProductGen {
public:
    /**
     * Members
     */
    static constexpr double maturity = NB_DAYS_TO_MATURITY/(double)BASE; /// The maturity of the product

    /**
     * Constructor (the payOff is initialized in the cpp)
     *
     * @param[in] pricer : the pricer for the product valuation
     * @param[in] assets : the list of underlying assets
     * @param[in] size : the number of underlying assets
     */
    Multimonde(PricerGen *pricer, Asset **assets, int size);

    /**
     * Overriding Methods
     */
    void MAJPortfolio();
};


#endif //PRICING_MULTIMONDE_HPP
