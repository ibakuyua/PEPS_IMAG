#include <iostream>
#include <string>
#include "../../Stats/Parser/ParseCSV.h"
int main(){

    std::string path = "../data/dataPEPS.csv";

    ParseCSV *parser = new ParseCSV(path,2016,02,31,10);

    std::cout << "Nombre de lignes : " << parser->outputData->m << std::endl;

    std::cout << "Nombre de colonnes : " << parser->outputData->n << std::endl;


    pnl_mat_print(parser->outputData);


    return 0;
}