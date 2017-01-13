//
// Created by Ruimy Benjamin Mac on 13/01/2017.
//

#include "StatsFactory.h"
#include <iostream>

StatsFactory::StatsFactory(PnlMat *quotes):quotes_(quotes){

    returns_ = pnl_mat_create_from_zero(quotes->m - 1, quotes->n);
    getReturnsFromQuotes();

    mean_ = pnl_vect_create_from_zero(quotes->n);
    getMeanFromReturns();

    covar_ = pnl_mat_create_from_zero(quotes->n,quotes->n);
    getCovarFromReturns();

    vol_ = pnl_vect_create_from_zero(quotes->n);
    getVolFromCovar();

    correl_ = pnl_mat_create_from_zero(quotes->n, quotes->n);
    getCorrelFromCovar();

}

StatsFactory::~StatsFactory(){

    pnl_vect_free(&vol_);
    pnl_mat_free(&returns_);
    pnl_mat_free(&correl_);
    pnl_mat_free(&covar_);

}

void StatsFactory::getReturnsFromQuotes() {
    double newValue = 0;
    double oldValue = 0;
    for(int j = 0; j < quotes_->n ; j++){
        oldValue = MGET(quotes_,0,j);
        for(int i = 0; i < quotes_->m - 1; i++){
            newValue = MGET(quotes_,i + 1,j);
            MLET(returns_,i,j) = log(newValue/oldValue);
            oldValue = newValue;
        }
    }
}

void StatsFactory::getMeanFromReturns() {

    double esp;
    for(int j = 0; j < returns_->n; j++){
        esp = 0;
        for(int i = 0; i < returns_->m; i++){
            esp+= MGET(returns_,i,j);
        }
        esp/=returns_->m;
        LET(mean_,j) = esp;
    }

}

void StatsFactory::getCovarFromReturns(){
    double esp1;
    double esp2;
    double coVar;
    for(int i = 0; i < returns_->n; i++){
        esp1 = GET(mean_,i);
        for(int j = 0; j <= i; j++){
            esp2 = GET(mean_,j);
            //GetCovar
            coVar = 0;
            for(int i2 = 0; i2 < returns_->m; i2++){
                coVar += (MGET(returns_,i2,i) - esp1)*(MGET(returns_,i2,j) - esp2);
            }
            MLET(covar_,i,j) = coVar / (returns_->m - 1);
        }
    }

    for(int i = 0; i < returns_->n; i++){
        for(int j = 0; j < i; j++){
            MLET(covar_,j,i) = MGET(covar_,i,j);
        }
    }

}

void StatsFactory::getVolFromCovar(){

    for(int i = 0; i < vol_->size; i++){
        LET(vol_,i) = sqrt(MGET(covar_,i,i));
    }

}

void StatsFactory::getCorrelFromCovar(){

    for(int i = 0; i < correl_->m; i++){
        for(int j = 0; j <= i; j++){
            MLET(correl_,i,j) = MGET(covar_,i,j) / GET(vol_,i) / GET(vol_,j);
            std::cout << "La valeur est pour i :  "  << i << ", j : " << j << " :" << MGET(correl_,i,j) << std::endl;
        }
    }
    for(int i = 0; i < correl_->m; i++){
        for(int j = 0; j < i; j++){
            MLET(correl_,j,i) = MGET(correl_,i,j);
        }
    }

}