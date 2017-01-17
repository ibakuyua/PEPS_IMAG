#ifndef PRICING_RATEMODELGEN_HPP
#define PRICING_RATEMODELGEN_HPP


#include "../FinancialProducts/Asset.hpp"

/**
 * RateModelGen[abstract]
 *
 * Represent a rate model
 */
class RateModelGen {
public:
    /**
     * Members
     */
    Change change; /// The change of the rate economy

    /**
     * Constructor/Destructor
     *
     * @param[in] change : the change of the rate economy
     */
    RateModelGen(Change change);
    virtual ~RateModelGen(){};

    /**
     * Abstract methods
     */
    /**
     * Permit to get the rate for a certain maturity
     *
     * @param[in] maturity : the maturity
     *
     * @return the rate at the maturity asked
     */
    virtual double GetRate(double maturity) const = 0;
    /**
     * Permit to get the integral of the rate
     *
     * @param[in] t : lower born for the integral
     * @param[in] maturity : upper born for the integral
     *
     * @return the integral between t and maturity of the rate model
     */
    virtual double GetIntegralRate(double t, double maturity) const = 0;
};


#endif //PRICING_RATEMODELGEN_HPP
