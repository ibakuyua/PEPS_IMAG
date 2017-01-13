#ifndef PEPS_IMAG_ASSET_HPP
#define PEPS_IMAG_ASSET_HPP

#include "pnl/pnl_vector.h"
#include <string>
#include <iostream>

typedef enum{
    EUR = 0,
    GBP = 1,
    USD = 2,
    HKD = 3,
    JPY = 4,
    AUD = 5
}Devise;

using namespace std;

/**
 * Asset : represent an asset for the product
 *
 * Can be modificated to add other information in the case where the
 * model is more complex
 */

class Asset {
public:
    string id;
    string name;
    Devise devise;
    double trend;
    double spot;
    double volatility; // sqrt(Sum_k sigma_(i,k)^2)


    Asset(string id, string name, Devise devise, double trend, double spot , double volatility);
    ~Asset();
};


#endif //PEPS_IMAG_ASSET_HPP
