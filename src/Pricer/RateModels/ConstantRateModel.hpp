#ifndef PRICING_CONSTANTRATEMODEL_HPP
#define PRICING_CONSTANTRATEMODEL_HPP


#include "RateModelGen.hpp"

#define FRR_DEFAULT 0.02

class ConstantRateModel : RateModelGen {
private:
    /**
     * Members
     */
    double parameter;
public:
    /**
     * Constructors
     */
    ConstantRateModel(double theFRR = FRR_DEFAULT);
    /**
     * Implement virtual function
     */
    double GetRate(double maturity) const;
};


#endif //PRICING_CONSTANTRATEMODEL_HPP
