#include "ProductGen.hpp"


ProductGen::ProductGen(string nom, PricerGen *pricer, Asset **assets, int size, int hedgingDateNb, PayOffFunction payOff)
    : nom(nom), pricer(pricer), assets(assets), size(size), hedgingDateNb(hedgingDateNb), payOff(payOff)
{
}

ProductGen::~ProductGen() {

}


void ProductGen::PricePortfolio(double t, double &price) const {
    //TODO With market
}

void ProductGen::PriceProduct(double t, double &price, double &ic) const {
    PnlMat *past = NULL;
    //past = Marche.GetPast(t);
    pricer->price(t, past, price, ic,payOff, parameters);
}
