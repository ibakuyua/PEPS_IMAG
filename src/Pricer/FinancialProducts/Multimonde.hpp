#ifndef PRICING_MULTIMONDE_HPP
#define PRICING_MULTIMONDE_HPP

#include "../ProductInfo/infoMultimonde.hpp"
#include "ProductGen.hpp"
#include "../Stats/StatsFactory.h"

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
    static double maturity;

    /**
     * Constructor : all the underlying asset are in the domestic economy
     *
     * @param[in] pricer : the pricer for the product valuation
     * @param[in] hedgingDateNb : the number of hedging date
     * @param[in] stats : the statistics in the order specified in Multimonde.hpp
     *
     * @details 0 : eurostock | 1 : Xftse      | 2 : Xp500 | 3 : Xhangseng
     *          4 : Xnikkei   | 5 : Xspasx200  | 6 : XRgbp | 7 : XRusd
     *          8 : XRcny     | 9 : XRjpy      | 10: XRaud
     */
    Multimonde(PricerGen *pricer, int hedgingDateNb, StatsFactory *stats);
    /**
     * Destructor
     */
    ~Multimonde();
    /**
     * Static methods
     */
     /**
      * GetAssetListFromStat : Permit to obtain the multimonde asset list with all the needed parameters
      *
      * @param stats using to estimate parameters
      * @param simuModel the modele used to simulate
      * @return the asset list of multimonde product
      */
     static AssetList *GetAssetListFromStat(StatsFactory *stats, ModelGen *simuModel);

     void UpdateAssetListFromStat(StatsFactory *stats, ModelGen *simuModel);
};


#endif //PRICING_MULTIMONDE_HPP
