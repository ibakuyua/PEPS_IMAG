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
     * Constructor 1 : all the underlying asset are in the domestic economy
     *
     * @param[in] pricer : the pricer for the product valuation
     * @param[in] hedgingDateNb : the number of hedging date
     * @param[in] assets : the underlying asset on the order specified in Multimonde.hpp
     *
     * @details 0 : eurostock | 1 : Xftse      | 2 : Xp500 | 3 : Xhangseng
     *          4 : Xnikkei   | 5 : Xspasx200  | 6 : XRgbp | 7 : XRusd
     *          8 : XRhkd     | 9 : XRjpy      | 10: XRaud
     */
    Multimonde(PricerGen *pricer, int hedgingDateNb, AssetList *assets);

    // Destructor
    ~Multimonde();
    /**
     * Overriding Methods
     */
    void MAJPortfolio();
};


#endif //PRICING_MULTIMONDE_HPP
