#ifndef PEPS_IMAG_PARSECSV_H
#define PEPS_IMAG_PARSECSV_H

#include "pnl/pnl_matrix.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>

using namespace std;

class ParseCSV {
public:

    PnlMat *inputData;

    ParseCSV(string path);

    ParseCSV(string path, int startYearOfEstimation,int startMonth, int startDay, int numberToParse);

    ~ParseCSV();


private:



};


#endif //PEPS_IMAG_PARSECSV_H
