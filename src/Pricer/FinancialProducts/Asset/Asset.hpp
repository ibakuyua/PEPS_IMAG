#ifndef PEPS_IMAG_ASSET_HPP
#define PEPS_IMAG_ASSET_HPP

#include "pnl/pnl_vector.h"
#include <string>
#include <iostream>

/**
 * Change : enum which represent the different changes
 */
typedef enum{
    EUR = 0,
    GBP = 1,
    USD = 2,
    CNY = 3,
    JPY = 4,
    AUD = 5
} Change;
std::ostream& operator<<(std::ostream& os,Change c);

using namespace std;

/**
 * Asset : represent an asset for the product
 *
 * Can be modificated to add other information in the case where the
 * product or model is more complex
 */

class Asset {
public:
    /**
     * Members
     */
    string id; /// Id of the asset
    string name; /// Complet name if the asset
    Change change; /// Economy of the asset
    double trend; /// Trend (mu) of the asset
    double spot; /// Spot (t=0) of the asset
    double volatility; /// volatility of the model (sqrt(Sum_k sigma_(i,k)^2) for classic B&S)

    /**
     * Methods
     */
    /**
     * Print : permit to print asset information
     */
    void Print();


    /**
     * Constructor / Destructor
     *
     * @param[in] id : the id of the asset
     * @param[in] name : the name of the asset
     * @param[in] change : the change of the asset economy
     * @param[in] trend : the trend of the asset
     * @param[in] spot : the spot (t=0) of the asset
     * @param[in] volatility : the volatility of the assert for the used model
     */
    Asset(string id, string name, Change change, double trend, double spot , double volatility);
    ~Asset();
};


#endif //PEPS_IMAG_ASSET_HPP
