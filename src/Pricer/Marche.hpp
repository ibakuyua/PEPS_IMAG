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
    HistoricalMultimonde = 1,
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
	static Marche *Instance(Change domesticChange = Change::EUR, ProductGen *product = NULL, int dateNb = 0);

    /**
     * ImportCotations : permit to fill the cotation market
     *
     * remarks : Add at the last column the 'asset' domestic ZC
     *
     * @param[in] type : type of cotations
     * @param[in] startYear : start year for the historical cotations
     * @param[in] startMonth : start month for the historical cotations
     * @param[in] startDay : start day for the historical cotations
     * @param[in] path : the data path for the historical cotations
     */
	void ImportCotations(CotationTypes type, int startYear = 0, int startMonth = 0, int startDay = 0, string path = "");

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


private:
    static Marche *instance; /// The unique instance of market
    CotationTypes type; /// The type of current cotations in the market
    int dateNb; /// Number of date in the market
    double dateStep; /// The market step (step between two indexes of cours)
    Change domesticChange; /// The domestic economy
    /**
     * Constructor/Destructor
     */
	Marche(ProductGen *product, Change domesticChange, int dateNb);
	~Marche();

    /**
     * Permit to get the historical cotations for multimonde21
     */
	void ImportHistoricalCotationsForMultimonde(int startYear, int startMonth, int startDay, string path);

};


#endif //PRICING_MARCHE_HPP
