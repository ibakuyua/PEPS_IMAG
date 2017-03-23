#ifndef PRICING_CONSTANTRATEMODEL_HPP
#define PRICING_CONSTANTRATEMODEL_HPP

#include "RateModelGen.hpp"

#define FRR_DEFAULT 0.02

class ConstantRateModel : public RateModelGen {
public:
    /**
     * Members
     */
    double parameter;
    /**
     * Constructors / Destructor
     */
	ConstantRateModel(Change devise, double theFRR = FRR_DEFAULT);
	~ConstantRateModel();
    /**
     * Implement virtual function
     */
	double GetRate(double maturity) const;

	virtual double GetIntegralRate(double t, double maturity) const;
};


#endif //PRICING_CONSTANTRATEMODEL_HPP
