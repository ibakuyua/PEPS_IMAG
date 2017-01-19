//
// Created by ruimyb on 1/17/17.
//
#include <iostream>
#include "../../Parser/ParseCSV.h"
#include <string>
int main(){

    std::string path = "Parser/dataPEPS.csv";

    ParseCSV *parser = new ParseCSV(path);

    std::cout << "Nombre de lignes : " << parser->inputData->m << std::endl;

    std::cout << "Nombre de colonnes : " << parser->inputData->n << std::endl;


    pnl_mat_print(parser->inputData);


    return 0;
}
