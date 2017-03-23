#include "ParseCSV.h"


ParseCSV::ParseCSV(string path) {
    outputData = pnl_mat_new();
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

        pnl_mat_resize(outputData,m,n);

        FILE.clear();
        FILE.seekg(FILE.beg);
        double tmpDouble;
        char tmpChar;
        getline(FILE,line);
        for(int i = 0; i < m; i++){
            while(FILE.get()!=delimiter){}
            for(int j = 0; j < n; j++){
                FILE >> tmpDouble;
                MLET(outputData,i,j) = tmpDouble;
                FILE >> tmpChar;
            }
        }

    }else
        std::cout << "Fichier CSV inexistant  !!!!!!!!!!" << endl;


}



ParseCSV::ParseCSV(string path, int startYear, int startMonth, int startDay, int numberToParse){

    //Creation of the pnl_mat
    outputData = pnl_mat_new();
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
        pnl_mat_resize(outputData,m,n);

        bool b = true;
        int tmpStartDay = 0;
        string tmpStartDayString;
        int tmpStartMonth = 0;
        string tmpStartMonthString;
        int tmpStartYear = 0;
        string tmpStartYearString;

        while(b){
            getline(FILE,line);
            if (!strcmp(line.c_str(),""))
                continue;
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


        double tmpDouble;
        char tmpChar;

        string tmpString;
        stringstream ss;
        stringstream sss;
        ss << line;
        getline(ss,tmpString,delimiter);
        for(int j = 0; j < n; j++){

            getline(ss,tmpString,delimiter);
            sss << tmpString;
            sss >> tmpDouble;
            sss.str("");
            sss.clear();
            MLET(outputData,0,j) = tmpDouble;

        }

        for(int i = 1; i < numberToParse; i++){
            //std::cout << "Itération : " << i << std::endl;
           // std::cout << MGET(inputData,i-1,1)
            while(FILE.get()!=delimiter){}
            for(int j = 0; j < n; j++){
                FILE >> tmpDouble;
                MLET(outputData,i,j) = tmpDouble;
                FILE >> tmpChar;
            }
        }
        FILE.close();

    }else
        std::cout << "Fichier CSV inexistant  !!!!!!!!!!" << endl;




}

ParseCSV::~ParseCSV() {
    pnl_mat_free(&outputData);
}

