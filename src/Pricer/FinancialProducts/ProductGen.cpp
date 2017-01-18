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
    pricer->Price(t, past, price, ic, payOff, parameters);
}


void ProductGen::PrintProduct() const {
    cout << "\n******************************************";
    cout << "\nProduct : " << this->nom;
    cout << "\nPricer : " ; this->pricer->PrintPricer();
    cout << "\n------------------------------------------";
    cout << "\nMaturity : " << this->pricer->maturity;
    cout << "\nNumber of hedging date : " << this->hedgingDateNb;
    cout << "\nNumber of underlying assets : " << this->size;
    for (int d = 0; d < this->size; ++d) {
        cout << "\n";
        assets[d]->PrintAsset();
    }
    cout << "\n------------------------------------------";
    cout << "\nParameters : ";
}
