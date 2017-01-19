#include "Multimonde.hpp"


Multimonde::Multimonde(PricerGen *pricer, int hedgingDateNb, AssetList *assets)
    : ProductGen("Multimonde21", pricer, hedgingDateNb, payOffMultimonde21,assets)
{
}

Multimonde::~Multimonde() {
}


void Multimonde::MAJPortfolio() {
    // TODO with market
}

