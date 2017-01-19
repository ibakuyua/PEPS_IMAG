#ifndef PEPS_IMAG_ASSETLIST_HPP
#define PEPS_IMAG_ASSETLIST_HPP

#include "Asset.hpp"

/**
 * AssetList : Represent a list of assets
 */
class AssetList {
public:
    /**
     * Members
     */
    int size; /// The number of asset in the list
    Asset **assets; /// The list of asset
    PnlMat *volatilityMat; /// Volatility matrix (cholesky of the calibrage in BS model)

    /**
     * Constructor / Destructor
     *
     * @param[in] size : the size of the list
     * @param[in] assets : the list of assets
     * @param[in] vol : the volatility matrix sigma
     */
    AssetList(int size, Asset **assets, PnlMat *vol);
    /**
     * Constructor
     *
     * @param[in] asset : one asset
     */
    AssetList(Asset *asset);
    ~AssetList();

    /**
     * Methods
     */
    /**
     * Print : permit to print the list of asset
     */
    void Print();

private:
    bool hasToDestroy = false;

};


#endif //PEPS_IMAG_ASSETLIST_HPP
