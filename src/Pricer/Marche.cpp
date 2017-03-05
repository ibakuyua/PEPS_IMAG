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
    // TODO faire les autres
    this->type = type;
    switch (type)
    {
        case CotationTypes::Simulated :
            product->pricer->simuModel->SimulateMarket(product->pricer->maturity, cours, this->dateNb, domesticChange);
            break;
        case CotationTypes::Historical :
        std::cout << int(product->pricer->maturity);
            //TODO NOW
            int maturity = product->pricer->maturity;
            int ti_minus1;
        std::cout << "Maturity : " << maturity << std::endl;
            double step = maturity/(double)(product->hedgingDateNb+1);

            pnl_mat_resize(cours,this->dateNb + 1,product->assets->size + 1);
            ParseCSV *parser = new ParseCSV(path,startYear,startMonth,startDay,this->dateNb + 1);
            //fill(&cours,parser->inputData);
            for(int j = 0; j < cours->n -1; j++){
                MLET(cours,0,j) = MGET(parser->outputData,0,j);
            }
            PNL_MSET(cours,0,cours->n-1, exp(product->pricer->simuModel->rateModels[Change::EUR]->GetIntegralRate(0.,ti_minus1+step)));


            for(int i = 1; i < cours->m; i++){
                ti_minus1 = (i-1) * step;
                for(int j = 0; j < cours->n - 1; j++){
                    MLET(cours,i,j) = MGET(parser->outputData,i,j);
                }
                PNL_MSET(cours,i,cours->n-1,exp(product->pricer->simuModel->rateModels[Change::EUR]->GetIntegralRate(0.,ti_minus1+step)));
            }

            delete(parser);
            break;
        //default:


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
