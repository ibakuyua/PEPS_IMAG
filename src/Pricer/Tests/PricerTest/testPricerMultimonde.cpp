#include <stdlib.h>
#include <iostream>
#include <assert.h>
#include <cstring>
#include "../../MultimondeFactory.hpp"

using namespace std;

int main(){
    int mcNb = 50000;

    clock_t start, end;
    start = clock();
    cout << "\n\n###### TEST OF THE MULTIMONDE PRICER ######\n\n";
    double prix, std;
    char *path = strdup(string("../data/dataPEPS.csv").c_str());
    MultimondeFactory::Price(0,2015,10,1,prix,std,path, mcNb);

    cout << "########################################\n\n";
    end = clock();
    std::cout << "Time of calculation : " << (double)(end-start)/CLOCKS_PER_SEC << "sec" << std::endl;
    return EXIT_SUCCESS;
}

