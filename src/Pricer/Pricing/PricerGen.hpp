#ifndef PRICING_PRICER_HPP
#define PRICING_PRICER_HPP


#include "../SimulationModels/ModelGen.hpp"
#include "pnl/pnl_matvect.h"

// Pointeur vers une fonction payOff d'un produit prenant en argument une matrice de cours
// et les paramètres du payOff
typedef double(*PayOffFunction)(PnlMat *path, PnlVect *parameters);

class PricerGen {
public:
    /**
     * Members
     */
    ModelGen *simuModel; /**< To simulate underlying asset */
    double maturity; /**< Maturity of the instrument to price */

    /**
     * Constructor / Destructor
     */
    PricerGen(double maturity, ModelGen *simuModel);
    virtual ~PricerGen();

    /**
     * Virtual methods
     */
    virtual void price(double t, PnlMat *past, double &price, double &ic,
                       PayOffFunction payOff, PnlVect *parameters = NULL) const = 0;
    virtual void delta(double t, PnlMat *past, PnlVect *delta,
                       PayOffFunction payOff, PnlVect* parameters = NULL) const = 0;

};


#endif //PRICING_PRICER_HPP
