//
// Created by ruimyb on 1/17/17.
//

#include "ParseCSV.h"


ParseCSV::ParseCSV(string path) {
    inputData = pnl_mat_new();
    char delimiter = ',';

    ifstream FILE(path,ios::in);
    if(FILE){
        char curChar = '0';
        string line;
        int m = 0;
        int n = 0;
        while(curChar != '\n'){
            if(curChar == delimiter){
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
            while(FILE.get()!=delimiter){}
            for(int j = 0; j < n; j++){
                FILE >> tmpDouble;
                MLET(inputData,i,j) = tmpDouble;
                FILE >> tmpChar;
            }
        }

    }else
        std::cout << "Fichier CSV inexistant  !!!!!!!!!!" << endl;


}



ParseCSV::ParseCSV(string path, int startYear, int startMonth, int startDay, int numberToParse){

    //Creation of the pnl_mat
    inputData = pnl_mat_new();
    //Delimiter used to parse
    char delimiter = ',';
    //Declaration + Openning of the FILE
    ifstream FILE(path,ios::in);

    if(FILE){
        char curChar = '0';
        string line;
        int m = numberToParse;
        int n = 0;

        //This part is used to find the number of columns
        while(curChar != '\n'){
            if(curChar == delimiter){
                n++;
            }
            curChar = FILE.get();
        }
        pnl_mat_resize(inputData,m,n);

        bool b = true;
        int tmpStartDay = 0;
        string tmpStartDayString;
        int tmpStartMonth = 0;
        string tmpStartMonthString;
        int tmpStartYear = 0;
        string tmpStartYearString;
        char* tmp;

        stringstream ss;
        while(b){
            getline(FILE,line);
            tmpStartDayString = line.substr(0,2);
            tmpStartDay = atoi(tmpStartDayString.c_str());

            tmpStartMonthString = line.substr(3,2);
            tmpStartMonth = atoi(tmpStartMonthString.c_str());

            tmpStartYearString = line.substr(6,4);
            tmpStartYear = atoi(tmpStartYearString.c_str());

            if(tmpStartYear < startYear) {
                b = false;
            }else if(tmpStartYear == startYear){
                if(tmpStartMonth < startMonth){
                    b = false;
                }else if(tmpStartMonth == startMonth){
                    if(tmpStartDay <= startDay)
                        b = false;
                }
            }

        }


        //FILE.clear();
        //FILE.seekg(FILE.beg);
        double tmpDouble;
        char tmpChar;

        string tmpString;
        stringstream sss;
        //getline(FILE,line);
        ss << line;
        getline(ss,tmpString,',');
        for(int j = 0; j < n; j++){

            getline(ss,tmpString,',');
            sss << tmpString;
            sss >> tmpDouble;
            sss.str("");
            sss.clear();
            MLET(inputData,0,j) = tmpDouble;

        }



        for(int i = 1; i < numberToParse; i++){
            while(FILE.get()!=delimiter){}
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

