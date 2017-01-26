#include "Call.hpp"

Call::Call(PricerGen *pricer, Asset *asset, int hedgingDateNb, double strike)
        : ProductGen("Call", pricer, hedgingDateNb, payOffCall, new AssetList(asset))
{
    this->parameters = pnl_vect_create_from_scalar(1,strike);
}

Call::~Call() {
    pnl_vect_free(&parameters);
    delete assets;
}

void Call::Print() const {
    ProductGen::Print();
    cout << "\nStrike : " << GET(this->parameters,0);
    cout << "\n******************************************";
}
