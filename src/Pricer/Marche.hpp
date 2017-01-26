#ifndef PRICING_MARCHE_HPP
#define PRICING_MARCHE_HPP

#include "FinancialProducts/ProductGen.hpp"

/*
 * CotationTypes
 *
 * Enum which represent the type of market cotations
 */
typedef enum {
    Real = 0,
    Historical = 1,
    Simulated = 2
}CotationTypes;

/**
 * Marche
 *
 * Represent the market with the product
 *
 * Singleton class
 */
class Marche {
public:
    ProductGen *product; /// The product to valuate
    PnlMat *cours; /// The cotations in the market (0 to T or 0 to t if real)

    /**
     * Instance : return an instance of the market
     *
     * remarks : permit to instance the market just one time
     *
     * @param[in] product : the product
     * @return a market
     */
    static Marche *Instance(ProductGen *product = NULL);

    /**
     * ImportCotations : permit to fill the cotation market
     *
     * @param[in] type : type of cotations
     */
    void ImportCotations(CotationTypes type);

    /**
     * Permit to obtain the cotations for the date t
     *
     * remarks : exception if t > current date of the market and cotation type is real
     *
     * @param[in] t : the date to get the cotations
     * @param[out] cotations : the result
     */
    void GetCotations(double t, PnlVect *cotations);

    /**
     * Permit to obtain the past matrix (in model step in case of withStepModel=true)
     *
     * remarks : if t and t+1 are in [stepModel_i,stepModel_i+1], return the same past matrix
     *           with just the last row different (S_t) if withStepModel = true
     *
     * @param[in] t : the date to get past matrix
     * @param[out] past : the past matrix
     * @param[in] withStepModel : determine if past is in model step or hedging step
     * @param[in] modelStepNb : the number of step in the model
     */
    void GetPastCotations(double t, PnlMat *past, bool withStepModel = false, int modelStepNb = 1);

    /**
     * SetTime : permit to set the real time of the market
     * @param t : the new date
     */
    inline static void SetTime(double t){Marche::t = t;}
    /**
     * GetTime : permit to return the time of the market
     * @return time
     */
    inline static double GetTime(){return Marche::t;}


private:
    static double t; /// Current time
    static Marche *instance; /// The unique instance of market
    CotationTypes type; /// The type of current cotations in the market
    double hedgingStep; /// The hedging step (step between two indexes of cours)
    double modelStep; /// The model step (step between two indexes of
    /**
     * Constructor/Destructor
     */
    Marche(ProductGen *product);
    ~Marche();
};


#endif //PRICING_MARCHE_HPP
