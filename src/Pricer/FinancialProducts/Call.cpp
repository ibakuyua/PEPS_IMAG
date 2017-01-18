#include "Call.hpp"

Call::Call(PricerGen *pricer, Asset **asset, int hedgingDateNb, double strike)
        : ProductGen("Call", pricer, 1 , hedgingDateNb, payOffCall, asset)
{
    parameters = pnl_vect_create_from_scalar(1,strike);
}

Call::~Call() {
    pnl_vect_free(&parameters);
}

void Call::MAJPortfolio() {
    //TODO pour tester la couverture ?
}


void Call::PrintProduct() const {
    ProductGen::PrintProduct();
    cout << "\nStrike : " << GET(this->parameters,0);
    cout << "\n******************************************";
}
