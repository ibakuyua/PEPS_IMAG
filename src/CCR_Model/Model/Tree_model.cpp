#include <stdlib.h>
#include <math.h>
#include "Tree_model.hpp"


Tree_model::Tree_model(double *maturity, int nb_step, int nb_index, double *spots, double *volatilities) {
    this->maturity = maturity;
    this->nb_step = nb_step;
    this->nb_index = nb_index;
    this->up_values = (double*) malloc(nb_index * sizeof(double));
    this->down_values = (double*) malloc(nb_index * sizeof(double));
    for (int i = 0; i < nb_index; ++i) {
        double step = (maturity[i+1]-maturity[i])/(double)nb_step;
        this->up_values[i] = exp(volatilities[i]*step);
        this->down_values[i] = 1./this->up_values[i];
    }
    // TODO
    this->stocks = (double*)malloc(0);
    Fill_tree(spots);
}

double Tree_model::Get_price() {
    return 0;
}

void Tree_model::display_tree() {
    // TODO
}

void Tree_model::Fill_tree(double *spots) {
    // TODO
}

Tree_model::~Tree_model() {
    free(this->up_values);
    free(this->down_values);
    free(this->stocks);
}
