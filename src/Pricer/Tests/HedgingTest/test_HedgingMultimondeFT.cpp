#include <stdlib.h>
#include <iostream>
#include <assert.h>
#include <fstream>
#include "../../MultimondeFactory.hpp"


using namespace std;
void computePnl(int hedgingNb);

int main(int argc, char** argv){
    int hedgingNb(5);
    if (argc > 1)
        hedgingNb = atoi(argv[1]);
    computePnl(hedgingNb);
    return EXIT_SUCCESS;
}

void computePnl(int hedgingNb) {

    cout << "\n\n###### TEST OF HEDGING MULTIMONDE (BACKTEST MARKET) ######\n\n";
    int monteCarloNb = 50000;
    double discrStep = 0.1;
    char *path = strdup(string("../data/backTest.csv").c_str());
    char *pathDatas = strdup(string("../data/dataPEPS.csv").c_str());
    MultimondeFactory::ForwardTest(hedgingNb,monteCarloNb,path, pathDatas, discrStep);
    // Delete
    free(path);
    free(pathDatas);
}