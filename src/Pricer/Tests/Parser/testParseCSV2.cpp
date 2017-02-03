#include <iostream>
#include <string>
#include "../../Stats/Parser/ParseCSV.h"
int main(){

    std::string path = "testPEPS.csv";

    ParseCSV *parser = new ParseCSV(path,1995,07,17,2);

    std::cout << "Nombre de lignes : " << parser->inputData->m << std::endl;

    std::cout << "Nombre de colonnes : " << parser->inputData->n << std::endl;


    pnl_mat_print(parser->inputData);


    return 0;
}