#include <stdlib.h>
#include <iostream>
#include <assert.h>
#include <cstring>
#include "../../MultimondeFactory.hpp"

using namespace std;

int main(){

    cout << "\n\n###### TEST OF THE MULTIMONDE PRICER ######\n\n";
    double prix, std;
    char *path = strdup(string("../data/dataPEPS.csv").c_str()); 
    MultimondeFactory::Price(0,2015,10,1,prix,std,path);

    cout << "########################################\n\n";
    return EXIT_SUCCESS;
}

