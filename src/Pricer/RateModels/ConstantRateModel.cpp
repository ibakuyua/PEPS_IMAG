#include "ConstantRateModel.hpp"


ConstantRateModel::ConstantRateModel(double theFRR) :parameter(theFRR){
}

double ConstantRateModel::GetRate(double maturity) const {
    return parameter;
}
