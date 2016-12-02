#ifndef PRICING_RATEMODELGEN_HPP
#define PRICING_RATEMODELGEN_HPP


class RateModelGen {
public:
    /**
     * Abstract methods
     */
    virtual double GetRate(double maturity) const = 0;
};


#endif //PRICING_RATEMODELGEN_HPP
