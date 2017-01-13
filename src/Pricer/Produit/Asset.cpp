#include "Asset.hpp"


Asset::Asset(string id, string name, Devise devise, double trend, double spot, double volatility)
        : id(id), name(name), devise(devise), trend(trend), spot(spot), volatility(volatility)
{
}

Asset::~Asset() {
}
