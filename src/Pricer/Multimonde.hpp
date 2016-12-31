#ifndef PRICING_MULTIMONDE_HPP
#define PRICING_MULTIMONDE_HPP

// Number of days from 01/10/2015 to each date of constatation
#define NB_DAYS_TO_CONSTATATION_1 373 // 01/10/2015 to 07/10/2016
#define NB_DAYS_TO_CONSTATATION_2 744 // 01/10/2015 to 13/10/2017
#define NB_DAYS_TO_CONSTATATION_3 1115 // 01/10/2015 to 19/10/2018
#define NB_DAYS_TO_CONSTATATION_4 1486 // 01/10/2015 to 25/10/2019
#define NB_DAYS_TO_CONSTATATION_5 1857 // 01/10/2015 to 30/10/2020
#define NB_DAYS_TO_CONSTATATION_6 2228 // 01/10/2015 to 05/11/2021

#define NB_DAYS_TO_MATURITY 2235 // 01/10/2015 to 12/11/2021
#define BASE 360

// Cap and Floor
#define FLOOR -0.15
#define CAP 0.15

// Nominal
#define NOMINAL 100

#include "Pricing/PricerGen.hpp"

class Multimonde {
public:
    /**
     * Members
     */
    static constexpr double maturity = NB_DAYS_TO_MATURITY/(double)BASE;
    PnlVect *composition;
    PricerGen *pricer;

    /**
     * Constructor
     */
    Multimonde(PricerGen *price);

    /**
     * Methods
     */
    void PricePortfolio(double t, double &price);
    void PriceMultimonde(double t, double &price, double &ic);
    void MAJPortfolio();
    /**
     * PayOff
     */
    static double payOff(PnlMat *path);

};


#endif //PRICING_MULTIMONDE_HPP
