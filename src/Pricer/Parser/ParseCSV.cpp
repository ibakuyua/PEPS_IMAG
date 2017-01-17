//
// Created by ruimyb on 1/17/17.
//

#include "ParseCSV.h"


ParseCSV::ParseCSV(string path) {
    inputData = pnl_mat_new();


    ifstream FILE(path,ios::in);
    if(FILE){

        char curChar = '0';
        string line;
        int m = 0;
        int n = 0;
        while(curChar != '\n'){
            if(curChar == ','){
                n++;
            }
            curChar = FILE.get();
        }
        while(getline(FILE,line))
            m++;

        pnl_mat_resize(inputData,m,n);

        FILE.clear();
        FILE.seekg(FILE.beg);
        double tmpDouble;
        char tmpChar;
        getline(FILE,line);
        for(int i = 0; i < m; i++){
            while(FILE.get()!=','){}
            for(int j = 0; j < n; j++){
                FILE >> tmpDouble;
                MLET(inputData,i,j) = tmpDouble;
                FILE >> tmpChar;
            }
        }

    }else
        std::cout << "Fichier CSV inexistant  !!!!!!!!!!" << endl;


}

ParseCSV::~ParseCSV() {
    pnl_mat_free(&inputData);
}

