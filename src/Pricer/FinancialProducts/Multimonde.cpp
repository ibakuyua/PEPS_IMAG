#include "Multimonde.hpp"


Multimonde::Multimonde(PricerGen *pricer, int hedgingDateNb, Asset **assets)
    : ProductGen("Multimonde21", pricer, 11, hedgingDateNb, payOffMultimonde21,assets)
{
}

Multimonde::~Multimonde() {
}


void Multimonde::MAJPortfolio() {
    // TODO with market
}

