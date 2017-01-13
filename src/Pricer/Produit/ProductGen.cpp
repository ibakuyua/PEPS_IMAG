#include "ProductGen.hpp"


ProductGen::ProductGen(PricerGen *pricer, Asset **assets, int size, int hedgingDateNb)
    : pricer(pricer), assets(assets), size(size), hedgingDateNb(hedgingDateNb)
{
}

ProductGen::~ProductGen() {

}
