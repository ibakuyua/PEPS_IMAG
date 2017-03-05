#ifndef PEPS_IMAG_PARSECSV_H
#define PEPS_IMAG_PARSECSV_H

#include "pnl/pnl_matrix.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>

using namespace std;

/**
 * Class which permit to parse a CSV of quotations
 */
class ParseCSV {
public:

    PnlMat *outputData; /// The data contained in the CSV converted in PnlMat

    /**
     * Constructor
     *
     * @param path : path to the file
     */
    ParseCSV(string path);
    /**
     * Constructor 2 (convert only from a start date to an end date)
     *
     * @param path : path to the file
     * @param startYearOfEstimation : start year date
     * @param startMonth : start month date
     * @param startDay : start day date
     * @param numberToParse : number of date to parse
     */
    ParseCSV(string path, int startYearOfEstimation,int startMonth, int startDay, int numberToParse);

    /**
     * Destructor
     */
    ~ParseCSV();
};


#endif //PEPS_IMAG_PARSECSV_H
