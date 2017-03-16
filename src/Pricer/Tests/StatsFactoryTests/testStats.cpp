#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_array.h"
#include <cmath>
#include "../../Stats/StatsFactory.h"
#include <iostream>
#include "../../Stats/Parser/ParseCSV.h"
int main(){

    cout << "\n\n###### TEST OF THE STAT FACTORY ######\n\n";
    PnlMat* quotes = pnl_mat_create_from_zero(4,3);
    ParseCSV*  parser = new ParseCSV("../data/dataPEPS.csv",2015,01,07,12);
    
    std::cout << "Test avec les données en dur :" << std::endl;
    MLET(quotes,0,0) = 2.0;
    MLET(quotes,0,1) = 5.2;
    MLET(quotes,0,2) = 3.1;
    MLET(quotes,1,0) = 3.2;
    MLET(quotes,1,1) = 6.8;
    MLET(quotes,1,2) = 3.1;
    MLET(quotes,2,0) = 1.5;
    MLET(quotes,2,1) = 5.9;
    MLET(quotes,2,2) = 3.2;
    MLET(quotes,3,0) = 2.8;
    MLET(quotes,3,1) = 3.1;
    MLET(quotes,3,2) = 3.4;


    //StatsFactory* statsFactory1 = new StatsFactory(quotes);
    StatsFactory* statsFactory2 = new StatsFactory(parser->outputData);
/*
    std::cout << "Matrice of quotes : " << std::endl;
    pnl_mat_print(statsFactory1->quotes_);

    std::cout << std::endl << "Matrice of returns : " << std::endl;
    pnl_mat_print(statsFactory1->logReturns_);


    std::cout << std::endl << "Matrice of Covariance : " << std::endl;
    pnl_mat_print(statsFactory1->covar_);

    std::cout << std::endl << "Vector of Volatility : " << std::endl;
    pnl_vect_print(statsFactory1->vol_);

    std::cout << std::endl << "Vector of Mean : " << std::endl;
    pnl_vect_print(statsFactory1->mean_);


    std::cout << std::endl << "Matrice of Correlation : " << std::endl;
    pnl_mat_print(statsFactory1->correl_);

*/
    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << "Test avec un fichier CSV en entrée !!!! " << std::endl;


    std::cout << "Matrice of quotes : " << std::endl;
    pnl_mat_print(statsFactory2->quotes_);

    /*std::cout << "Matrice of quotes in EUR : " << std::endl;
    pnl_mat_print(statsFactory2->quotesInEUR_);*/

    std::cout << std::endl << "Matrice of returns : " << std::endl;
    pnl_mat_print(statsFactory2->logReturns_);


    std::cout << std::endl << "Matrice of Covariance : " << std::endl;
    pnl_mat_print(statsFactory2->covar_);

    std::cout << std::endl << "Vector of Volatility : " << std::endl;
    pnl_vect_print(statsFactory2->vol_);

    std::cout << std::endl << "Vector of Mean : " << std::endl;
    pnl_vect_print(statsFactory2->mean_);


    std::cout << std::endl << "Matrice of Correlation : " << std::endl;
    pnl_mat_print(statsFactory2->correl_);
    std::cout << std::endl;

    delete parser;
    pnl_mat_free(&quotes);



    return EXIT_SUCCESS;
}

