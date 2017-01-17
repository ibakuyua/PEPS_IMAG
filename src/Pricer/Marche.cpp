#include "Marche.hpp"
#include "Pricing/MonteCarloPricer.hpp"

double Marche::t = 0;
Marche *Marche::instance = NULL;
PnlVect *Marche::cours_t = NULL;

Marche *Marche::Instance(ProductGen *product) {
    if (instance == NULL)
        instance = new Marche(product);
    return instance;
}

void Marche::ImportCotations(CotationTypes type) {
    // TODO faire les autres
    this->type = type;
    switch (type)
    {
        case CotationTypes::Simulated :
            product->pricer->simuModel->SimulateMarket(product->pricer->maturity, cours, product->hedgingDateNb);
            break;
        default:
            break;
    }
}


void Marche::GetCotations(double t, PnlVect* cotations) {
    // TODO mettre à jours cours_t pour qu'il contienne la bonne ligne de
    // TODO cours i.e. trouver le bon indice en fonction de t
}

Marche::Marche(ProductGen *product)
    : product(product)
{
    cours = pnl_mat_create_from_zero(product->size,product->hedgingDateNb);
    cours_t = pnl_vect_create_from_zero(product->size);
}

Marche::~Marche() {
    pnl_mat_free(&cours);
    pnl_vect_free(&cours_t);
}
