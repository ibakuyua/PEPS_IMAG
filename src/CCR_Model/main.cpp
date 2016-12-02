#include <iostream>
#include "Model/Tree_model.hpp"

void fill_volatilities(double *pDouble);

void display_parameters(double *pDouble, int index, int step, double *spots, double *volatilities);

void fill_spot(double *pDouble);

void fill_maturity(double *pDouble);

using namespace std;

int main() {
    /////// Parameters ///////



    cout << "\n---- Multimonde contract ----\n\n";
    int nb_index = 1;
    int nb_step = 10;
    double *maturity = (double*)malloc(nb_index+1*sizeof(double));
    double *spots = (double*)malloc(nb_index+1*sizeof(double));
    double *volatilities = (double*)malloc(nb_index+1*sizeof(double));
    fill_maturity(maturity);
    fill_spot(spots);
    fill_volatilities(volatilities);
    display_parameters(maturity, nb_index, nb_step, spots, volatilities);

    Tree_model *model = new Tree_model(maturity,nb_step,nb_index,spots,volatilities);
    double price = model->Get_price();

    cout << "\n---> Price at 0 : " << price << "\n\n-----------------------------------";

    free(maturity);
    free(spots);
    free(volatilities);
    delete model;
    exit(0);
}

void fill_maturity(double *maturities) {
    maturities[0] = 0;
    maturities[1] = 4;
}

void fill_spot(double *spots) {
    spots[0] = 100;
}

void display_parameters(double *maturities, int index, int step, double *spots, double *volatilities) {
    cout << "\n****************** PARAMETERS *****************\n";
    cout << "\nNB INDEX     : " << index;
    cout << "\nNB STEP      : " << step;
    cout << "\nMaturities   : ";
    for (int i = 0; i <= index; ++i)
        cout << " " << maturities[i] << " |";
    cout << "\nSpots        : ";
    for (int i = 0; i < index; ++i)
        cout << " " << spots[i] << " |";
    cout << "\nVolatilities : ";
    for (int i = 0; i < index; ++i)
        cout << " " << volatilities[i] << " |";
    cout << "\n\n************************************************\n";
}

void fill_volatilities(double *volatilities) {
    volatilities[0] = 0.2;
}