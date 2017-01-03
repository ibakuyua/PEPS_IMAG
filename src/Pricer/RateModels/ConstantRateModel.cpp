#include "ConstantRateModel.hpp"


ConstantRateModel::ConstantRateModel(double theFRR) :parameter(theFRR){
}

double ConstantRateModel::GetRate(double maturity) const {
    return parameter;
}


double ConstantRateModel::GetIntegralRate(double t, double maturity) const {
    return (maturity - t) * parameter;
}


ConstantRateModel::~ConstantRateModel() {
}
