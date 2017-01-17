#include "Multimonde.hpp"

Multimonde::Multimonde(PricerGen *pricer, Asset **assets, int size)
        : ProductGen("Multimonde21", pricer, assets, size, NB_DAYS_TO_MATURITY, payOffMultimonde21_simple)
{
}

void Multimonde::MAJPortfolio() {
    // TODO with market
}

