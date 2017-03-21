#ifndef PRICING_CONSTANTRATEMODEL_HPP
#define PRICING_CONSTANTRATEMODEL_HPP
#define DLLEXP   __declspec( dllexport )

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
	DLLEXP ConstantRateModel(Change devise, double theFRR = FRR_DEFAULT);
	DLLEXP ~ConstantRateModel();
    /**
     * Implement virtual function
     */
	DLLEXP double GetRate(double maturity) const;

	DLLEXP virtual double GetIntegralRate(double t, double maturity) const;
};


#endif //PRICING_CONSTANTRATEMODEL_HPP
