#include "ProductGen.hpp"


ProductGen::ProductGen(string nom, PricerGen *pricer, int hedgingDateNb, PayOffFunction payOff,
                        AssetList *assets, PnlVect* parameters)
    : nom(nom), pricer(pricer), assets(assets), hedgingDateNb(hedgingDateNb)
        , payOff(payOff), parameters(parameters)
{
    if (assets != NULL)
        this->pricer->simuModel->SetAssets(assets); // Obligation to do this
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


void ProductGen::Print() const {
    cout << "\n******************************************";
    cout << "\nProduct : " << this->nom;
    cout << "\nPricer : " ; this->pricer->PrintPricer();
    cout << "\n------------------------------------------";
    cout << "\nMaturity : " << this->pricer->maturity;
    cout << "\nNumber of hedging date : " << this->hedgingDateNb;
    cout << "\n"; assets->Print();
    cout << "\n------------------------------------------";
    cout << "\nParameters : ";
}
