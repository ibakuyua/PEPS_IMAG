#include "Asset.hpp"


Asset::Asset(string id, string name, Change change, double trend, double spot, double volatility)
        : id(id), name(name), change(change), trend(trend), spot(spot), volatility(volatility)
{
}


void Asset::Print() {
    cout << "[ " <<this->id << " ] : " << this->name << " (" << this->change << ") ";
    cout << "\n\t Spot : " << this->spot << " | Trend : " << this->trend << " | Vol : " << this->volatility;
}

Asset::~Asset() {
}
