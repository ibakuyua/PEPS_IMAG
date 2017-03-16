#include "Marche.hpp"
#include "Stats/Parser/ParseCSV.h"
#include <cstdlib>
Marche *Marche::instance = NULL;

Marche *Marche::Instance(Change domesticChange, ProductGen *product, int dateNb) {
    if (instance == NULL && product != NULL)
        instance = new Marche(product, domesticChange, dateNb);
    return instance;
}
void fill(PnlMat **cours , PnlMat *data);

void fill(PnlMat **cours , PnlMat *data){

    int m = (*cours)->m;
    int n = (*cours)->n;
    int i1 = 0;
    int z = data->m;
    int step = data->m/m;

    for(int i = 0; i < m;i++){

        for(int j = 0; j < n;j++){
            MLET(*cours,i,j) = MGET(data,i1,j);
        }
        std::cout << "i1 = " << i1 << std::endl;
        i1 = i1 + step;
    }

}

void Marche::ImportCotations(CotationTypes type,int startYear,int startMonth,int startDay, string path) {
    this->type = type;
    switch (type)
    {
        case CotationTypes::Simulated :
            product->pricer->simuModel->SimulateMarket(product->pricer->maturity, cours, this->dateNb, domesticChange);
            break;
        case CotationTypes::HistoricalMultimonde :
            ImportHistoricalCotationsForMultimonde(startYear, startMonth, startDay, path);
            break;
        default:
            break;
    }
}


void Marche::GetCotations(double t, PnlVect *cotations) {
    if (instance == NULL)
        return;
    int hedgingIndex = (int)(t/dateStep);
    pnl_mat_get_row(cotations,cours,hedgingIndex);
}

void Marche::GetPastCotations(double t, PnlMat *past, bool withStepModel, int modelStepNb) {
    if (instance == NULL)
        return;
    int dateIndex = (int)(t / dateStep);
    if (withStepModel){
        double step = product->pricer->maturity / (double)modelStepNb;
        // Spot initialisation
        PnlVect *quotes_t = pnl_vect_new();
        pnl_mat_get_row(quotes_t,cours,0);
        pnl_mat_resize(past,1,cours->n);
        pnl_mat_set_row(past,quotes_t,0);
        int k = 1;
        for (int i = 1; i < dateIndex; ++i) {
            if (i % (int)(step/dateStep) == 0)
            {
                pnl_mat_get_row(quotes_t,cours,i);
                pnl_mat_add_row(past,k,quotes_t);
                k++;
            }
        }
        if (dateIndex > 0) { // add spot
            pnl_mat_get_row(quotes_t,cours,dateIndex);
            pnl_mat_add_row(past,k,quotes_t);
        }
        pnl_vect_free(&quotes_t);
    }
    else
        pnl_mat_extract_subblock(past,cours,0,dateIndex+1,0,cours->n);
}

Marche::Marche(ProductGen *product, Change domesticChange, int dateNb)
        : product(product)
{
    if (dateNb == 0)
        dateNb = (int)product->pricer->maturity;
    this->dateNb = dateNb;
    cours = pnl_mat_create_from_zero(product->hedgingDateNb+1, product->assets->size+1);
    dateStep = product->pricer->maturity / dateNb;
    this->domesticChange = domesticChange;
}

Marche::~Marche() {
    pnl_mat_free(&cours);
}

void Marche::ImportHistoricalCotationsForMultimonde(int startYear, int startMonth, int startDay, string path) {
    // Resize
    pnl_mat_resize(cours,this->dateNb + 1,product->assets->size + 1);
    // Step initialization
    double maturity = product->pricer->maturity;
    double step = maturity/(double)this->dateNb;
    double ti;
    // Define the association between change and index of the CSV


    // Parse the CSV
    ParseCSV *parser = new ParseCSV(path,startYear,startMonth,startDay,this->dateNb + 1);
    // Fill the datas
    // For each time (/!\ be carreful of the order in the CSV file) //
    for (int i = 0; i < cours->m; ++i) {
        ti = (cours->m - 1 - i) * step;
        // For eurostock
        MLET(cours,cours->m - 1 - i,0) = MGET(parser->outputData,i,0);
        // For each foreign index : X * I
        for (int d = 1; d < 6; ++d)
            MLET(cours,cours->m - 1 - i,d) =
                    MGET(parser->outputData,i,d+5) * MGET(parser->outputData,i,d);
        // For each free risk asset : X * R
        for (int d = 6; d < 11; ++d) {
            MLET(cours,cours->m - 1 - i,d) = MGET(parser->outputData,i,d)
                              * exp(product->pricer->simuModel->
                    rateModels[product->assets->assets[d]->isChange.second]->GetIntegralRate(0,ti)
                                   );
        }
        // For domestic free risk
        MLET(cours,cours->m - 1 - i,11) =
                exp(product->pricer->simuModel->rateModels[EUR]->GetIntegralRate(0,ti));
    }
    // Change the spot value for the multimonde21
    for (int d = 0; d < 11; ++d)
        product->assets->assets[d]->spot = MGET(cours,0,d);

    delete(parser);
}
