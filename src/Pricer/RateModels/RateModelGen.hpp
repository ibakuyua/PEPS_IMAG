#ifndef PRICING_RATEMODELGEN_HPP
#define PRICING_RATEMODELGEN_HPP


class RateModelGen {
public:
    /**
     * Abstract methods
     */
    virtual ~RateModelGen(){};
    /**
     * Permit to get the rate for a certain maturity
     *
     * @param[in] maturity : the maturity
     * @return the rate at the maturity asked
     */
    virtual double GetRate(double maturity) const = 0;
    /**
     * Permit to get the integral of the rate
     *
     * @param[in] t : lower born for the integral
     * @param[in] maturity : upper born for the integral
     */
    virtual double GetIntegralRate(double t, double maturity) const = 0;
};


#endif //PRICING_RATEMODELGEN_HPP
