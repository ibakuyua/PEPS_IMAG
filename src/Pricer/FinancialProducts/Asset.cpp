#include "Asset.hpp"


Asset::Asset(string id, string name, Change change, double trend, double spot, double volatility)
        : id(id), name(name), change(change), trend(trend), spot(spot), volatility(volatility)
{
}

Asset::~Asset() {
}
