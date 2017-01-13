#include "Marche.hpp"

double Marche::t = 0;
Marche *Marche::instance = NULL;
PnlVect *Marche::cours_t = NULL;

Marche *Marche::Instance(ProductGen *product) {
    if (instance == NULL)
        instance = new Marche(product);
    return instance;
}

void Marche::ImportCotations(CotationTypes type) {
    // TODO
}


PnlVect *Marche::GetCotations(double t) {
    // TODO mettre à jours cours_t pour qu'il contienne la bonne ligne de
    // TODO cours i.e. trouver le bon indice en fonction de t
    return cours_t;
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
