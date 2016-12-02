#include "Multimonde.hpp"

//TODO


Multimonde::Multimonde(PricerGen *pricer)
        : pricer(pricer)
{
}

void Multimonde::PricePortfolio(double t, double &price) {

}

void Multimonde::PriceMultimonde(double t, double &price, double &ic) {
    PnlMat *past;
    //past = Marche.GetPast(t);
    PayOffFunction payOffFunction = payOff;
    pricer->price(t, past, price, ic, payOffFunction);
}

void Multimonde::MAJPortfolio() {

}

double Multimonde::payOff(PnlMat *path) {
    return 0;
}
