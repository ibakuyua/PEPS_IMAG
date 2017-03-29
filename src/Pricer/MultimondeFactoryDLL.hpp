#ifndef PEPS_IMAG_MULTIMONDEFACTORYDLL_HPP
#define PEPS_IMAG_MULTIMONDEFACTORYDLL_HPP

#define DLLEXP   __declspec( dllexport )

class MultimondeFactoryDLL {
    /**
     * Price : permit to price at t the multimonde21 with a confidence interval
     *
     * @param[in] t : time in working days after 01/10/15
     * @param[in] year  : year of the date
     * @param[in] month  : month of the date
     * @param[in] day  : day of the date
     * @param[out] price : price of the product
     * @param[out] std : standard deviation of monte carlo method
     * @param[in] pathDatas : the path for the datas.csv market
     */
    DLLEXP static void PriceDLL(double t, int year, int month, int day, double &price, double &std, char* pathDatas);
    /**
     * Hedge : permit to give the composition at t of the hedging portfolio
     *
     * @param[in] t : time in working days after 01/10/15
     * @param[in] year  : year of the date
     * @param[in] month  : month of the date
     * @param[in] day  : day of the date
     * @param[out] compo : composition in this order X_EUROSTOCK, X_FTSE, X_P500, X_HANGSENG
     *                     X_NIKKEI, X_SPASX200, X_ZCGBP, X_ZCUSD, X_ZCHKD, X_ZCJPY, X_ZCAUD, ZCEUR
     *                     Must be allocated with a length of 12
     * @param[out] std : standard deviation for each delta in the previous order
     *                     Must be allocated with a length of 12
     * @param[in] pathDatas : the path for the market datas csv file
     */
    DLLEXP static void HedgeDLL(double t, int year, int month, int day, double *compo, double *std, char* pathDatas);
    /**
     * BackTest : permit to launch a backtest
     *
     * @param hedgingNb : Hedging number
     * @param MCnb : Number of MonteCarlo sample
     * @param path : The path for the CSV results
     * @param pathDatas : The path for the datas.csv
     * @param discrStep : Discretisation step for the delta method (default 0.1)
     */
    DLLEXP static void BackTestDLL(int hedgingNb, int MCnb, char *path, char *pathDatas, double discrStep = 0.1);
    /**
     * ForwardTest : permit to laucnh a forward test
     *
     * @param hedgingNb : Hedging number
     * @param MCnb : Number of MonteCarlo sample
     * @param path : The path for the CSV results
     * @param pathDatas : The path for the datas.csv
     * @param discrStep : Discretisation step for the delta method (default 0.1)
     */
    DLLEXP static void ForwardTestDLL(int hedgingNb, int MCnb, char *path, char *pathDatas, double discrStep = 0.1);
};


#endif //PEPS_IMAG_MULTIMONDEFACTORYDLL_HPP
