#include "ChangeZC.hpp"


ChangeZC::ChangeZC(string id, string name, Change domesticChange, Change foreignChange, double trend, double spot,
                   double volatility)
        : Asset(id,name,domesticChange,trend,spot,volatility), foreignChange(foreignChange)
{
    this->isChange = pair<bool,Change>(true,foreignChange);
}
