#include <stdlib.h>
#include <iostream>
#include <assert.h>
#include <cstring>
#include "../../MultimondeFactory.hpp"

using namespace std;

int main(){
    int mcNb = 50000;
    double discr = 0.1;
    double *compo = (double*)malloc(12* sizeof(double));
    double *std = (double*)malloc(12* sizeof(double));
    clock_t start, end;
    start = clock();
    cout << "\n\n###### TEST OF THE MULTIMONDE DELTA ######\n\n";
    char *path = strdup(string("../data/dataPEPS.csv").c_str());
    MultimondeFactory::Hedge(0,2015,10,1,compo, std, path,mcNb, discr);

    cout << "########################################\n\n";
    end = clock();
    std::cout << "Time of calculation : " << (double)(end-start)/CLOCKS_PER_SEC << "sec" << std::endl;
    return EXIT_SUCCESS;
}
