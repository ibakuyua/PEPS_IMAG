#include "MonteCarloPricer.hpp"


MonteCarloPricer::MonteCarloPricer(ModelGen *simuIndex, ModelGen *simuChange, int nbSample)
        : PricerGen(simuIndex, simuChange)
{
    this->nbSample = nbSample;
}

void MonteCarloPricer::price(double t, PnlMat *past, double &price, double &ic, PayOffFunction payOff) const {

}

void MonteCarloPricer::delta(double t, PnlMat *past, PnlVect *delta, PayOffFunction payOff) const {
    // TODO
}
