//
// Created by Arnaud Ibakuyumcu on 02/12/2016.
//

#include "MonteCarloPricer.hpp"


MonteCarloPricer::MonteCarloPricer(ModelGen *simuIndex, ModelGen *simuChange, int nbSample)
        : PricerGen(simuIndex, simuChange)
{
    this->nbSample = nbSample;
}

void MonteCarloPricer::price(double t, PnlMat *past, double &price, double &ic, PayOffFunction payOff) const {
    // TODO (utilisation du pointeur de fonction : payOff(path)
}

void MonteCarloPricer::delta(double t, PnlMat *past, PnlVect *delta, PayOffFunction payOff) const {
    // TODO
}
