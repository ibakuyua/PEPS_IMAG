#include "ProductGen.hpp"
#include "../Marche.hpp"


ProductGen::ProductGen(string nom, PricerGen *pricer, int hedgingDateNb, PayOffFunction payOff,
                        AssetList *assets, PnlVect* parameters)
    : nom(nom), pricer(pricer), assets(assets), hedgingDateNb(hedgingDateNb)
        , payOff(payOff), parameters(parameters)
{
    if (assets != NULL)
        this->pricer->simuModel->SetAssets(assets); // Obligation to do this
    composition = pnl_vect_create(assets->size+1);
    icComposition = pnl_vect_create(assets->size);
    pastQuotes = pnl_mat_new();
    spotQuotes = pnl_vect_new();
}

ProductGen::~ProductGen() {
    pnl_vect_free(&composition);
    pnl_vect_free(&icComposition);
}


void ProductGen::PricePortfolio(double t, double &price) const {
    Marche *market = Marche::Instance(); // There is only one instance of market
    market->GetCotations(t,spotQuotes);
    price = pnl_vect_scalar_prod(composition,spotQuotes);
}

void ProductGen::PriceProduct(double t, double &price, double &ic) const {
    Marche *market = Marche::Instance();
    market->GetPastCotations(t,pastQuotes,true,pricer->nbTimeStep);
    pricer->Price(t, pastQuotes, price, ic, payOff, parameters);
}

void ProductGen::UpdatePortfolio(double t) {
    Marche *market = Marche::Instance();
    market->GetPastCotations(t,pastQuotes,true,pricer->nbTimeStep);
    market->GetCotations(t,spotQuotes);
    pricer->Delta(t,pastQuotes,composition,icComposition,payOff,parameters);
    pnl_vect_resize(composition,composition->size+1);
    PNL_SET(composition,composition->size-1,0.); // For the scalar prod (0 * R1(t))
    double prix, ic;
    pricer->Price(t,pastQuotes,prix,ic,payOff,parameters);
    double compoFreeRiskAsset = prix - pnl_vect_scalar_prod(composition,spotQuotes);
    compoFreeRiskAsset /= GET(spotQuotes,spotQuotes->size-1); // R1(t)
    PNL_SET(composition,composition->size-1,compoFreeRiskAsset);
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
