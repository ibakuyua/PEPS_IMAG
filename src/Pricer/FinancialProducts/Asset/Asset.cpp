#include "Asset.hpp"


std::ostream& operator<<(std::ostream& os,Change c){
   const char* txt[] = {"EUR", "GBP", "USD", "HKD", "JPY", "AUD"};
   return os << txt[(int)c];
}

Asset::Asset(string id, string name, Change change, double trend, double spot, double volatility)
        : id(id), name(name), change(change), trend(trend), spot(spot), volatility(volatility)
{
    isChange = pair<bool,Change>(false,change);
}


void Asset::Print() {
    cout << "[ " <<this->id << " ] : " << this->name << " (" << this->change << ") ";
    cout << "\n\t Spot : " << this->spot << " | Trend : " << this->trend << " | Vol : " << this->volatility;
}

Asset::~Asset() {
}
