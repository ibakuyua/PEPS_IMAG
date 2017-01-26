#include "ProductGen.hpp"
#include "../Marche.hpp"


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
    PnlVect *quotes = pnl_vect_new();
    Marche *market = Marche::Instance(); // There is only one instance of market
    market->GetCotations(t,quotes);
    price = pnl_vect_scalar_prod(composition,quotes);
    pnl_vect_free(&quotes);
}

void ProductGen::PriceProduct(double t, double &price, double &ic) const {
    PnlMat *past = pnl_mat_new();
    Marche *market = Marche::Instance();
    market->GetPastCotations(t,past,true,pricer->nbTimeStep);
    pricer->Price(t, past, price, ic, payOff, parameters);
    pnl_mat_free(&past);
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
