#include "Marche.hpp"

double Marche::t = 0;
Marche *Marche::instance = NULL;

Marche *Marche::Instance(ProductGen *product) {
    if (instance == NULL && product != NULL)
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


void Marche::GetCotations(double hedgingDate, PnlVect *cotations) {
    if (instance == NULL)
        return;
    int hedgingIndex = (int)(hedgingDate/hedgingStep);
    pnl_mat_get_row(cotations,cours,hedgingIndex);
}

void Marche::GetPastCotations(double hedgingDate, PnlMat *past, bool withStepModel, int modelStepNb) {
    if (instance == NULL)
        return;
    int hedgingIndex = (int)(hedgingDate / hedgingStep);
    double step = product->pricer->maturity / (double)modelStepNb;
    if (withStepModel){
        // Spot initialisation
        PnlVect *quotes_t = pnl_vect_new();
        pnl_mat_resize(past,1,product->assets->size);
        pnl_mat_set_row(past,product->pricer->simuModel->spot,1);
        int k = 1;
        for (int i = 1; i <= hedgingDate; ++i) {
            if (i % (int)(step/hedgingStep) == 0)
            {
                pnl_mat_get_row(quotes_t,cours,i);
                pnl_mat_add_row(past,k,quotes_t);
                k++;
            }
        }
        pnl_vect_free(&quotes_t);
    }
    else
        pnl_mat_extract_subblock(past,cours,0,hedgingIndex,0,cours->n);
}

Marche::Marche(ProductGen *product)
    : product(product)
{
        cours = pnl_mat_create_from_zero(product->hedgingDateNb, product->assets->size);
        hedgingStep = product->pricer->maturity / product->hedgingDateNb;
}

Marche::~Marche() {
    pnl_mat_free(&cours);
}
