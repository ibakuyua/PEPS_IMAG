#include "StatsFactory.h"
#include <iostream>

StatsFactory::StatsFactory(PnlMat *quotes):quotes_(quotes){

    logReturns_ = pnl_mat_create_from_zero(quotes->m - 1, quotes->n);
    ComputeLogReturnsFromQuotes();

    mean_ = pnl_vect_create_from_zero(quotes->n);
    ComputeMeanFromLogReturns();

    covar_ = pnl_mat_create_from_zero(quotes->n,quotes->n);
    ComputeCovarFromLogReturns();

    vol_ = pnl_vect_create_from_zero(quotes->n);
    ComputeVolFromCovar();

    correl_ = pnl_mat_create_from_zero(quotes->n, quotes->n);
    ComputeCorrelFromCovar();

}

//Ajout benji
void StatsFactory::UpdateStatsFactory(){


    pnl_mat_resize(logReturns_,quotes_->m - 1, quotes_->n);
    ComputeLogReturnsFromQuotes();

    pnl_vect_resize(mean_,quotes_->n);
    ComputeMeanFromLogReturns();

    pnl_mat_resize(covar_,quotes_->n,quotes_->n);
    ComputeCovarFromLogReturns();

    pnl_vect_resize(vol_,quotes_->n);
    ComputeVolFromCovar();

    pnl_mat_resize(correl_,quotes_->n, quotes_->n);
    ComputeCorrelFromCovar();

}



StatsFactory::~StatsFactory(){

    pnl_vect_free(&vol_);
    pnl_mat_free(&logReturns_);
    pnl_mat_free(&correl_);
    pnl_mat_free(&covar_);

}
/*
void StatsFactory::getQuotesInEUR() {

    for(int i = 0; i < quotes_->m; i++){
        for(int j = 0; j < quotes_->n;j++){
            if(j == 1){
                MLET(quotesInEUR_,i,j) = MGET(quotes_,i,j) * MGET(quotes_,i,6);
            }else if(j == 2){
                MLET(quotesInEUR_,i,j) = MGET(quotes_,i,j) * MGET(quotes_,i,7);
            }else if(j == 3){
                MLET(quotesInEUR_,i,j) = MGET(quotes_,i,j) * MGET(quotes_,i,8);
            }else if(j == 4){
                MLET(quotesInEUR_,i,j) = MGET(quotes_,i,j) * MGET(quotes_,i,9);
            }else if(j == 5){
                MLET(quotesInEUR_,i,j) = MGET(quotes_,i,j) * MGET(quotes_,i,10);
            }else{
                MLET(quotesInEUR_,i,j) = MGET(quotes_,i,j);
            }
        }
    }

}
 */

void StatsFactory::ComputeLogReturnsFromQuotes() {
    double newValue = 0;
    double oldValue = 0;
    for(int j = 0; j < quotes_->n ; j++){
        oldValue = MGET(quotes_,0,j);
        for(int i = 0; i < quotes_->m - 1; i++){
            newValue = MGET(quotes_,i + 1,j);
            MLET(logReturns_, i, j) = log(newValue / oldValue);
            oldValue = newValue;
        }
    }
}

void StatsFactory::ComputeMeanFromLogReturns() {

    double esp;
    for(int j = 0; j < logReturns_->n; j++){
        esp = 0;
        for(int i = 0; i < logReturns_->m; i++){
            esp+= MGET(logReturns_, i, j);
        }
        esp/= logReturns_->m;
        LET(mean_,j) = esp;
    }

}

void StatsFactory::ComputeCovarFromLogReturns(){
    double esp1;
    double esp2;
    double coVar;
    for(int i = 0; i < logReturns_->n; i++){
        esp1 = GET(mean_,i);
        for(int j = 0; j <= i; j++){
            esp2 = GET(mean_,j);
            //GetCovar
            coVar = 0;
            for(int i2 = 0; i2 < logReturns_->m; i2++){
                coVar += (MGET(logReturns_, i2, i) - esp1) * (MGET(logReturns_, i2, j) - esp2);
            }
            MLET(covar_,i,j) = coVar / (logReturns_->m - 1);
        }
    }

    for(int i = 0; i < logReturns_->n; i++){
        for(int j = 0; j < i; j++){
            MLET(covar_,j,i) = MGET(covar_,i,j);
        }
    }

}

void StatsFactory::ComputeVolFromCovar(){

    for(int i = 0; i < vol_->size; i++){
        LET(vol_,i) = sqrt(MGET(covar_,i,i));
    }

}

void StatsFactory::ComputeCorrelFromCovar(){

    for(int i = 0; i < correl_->m; i++){
        for(int j = 0; j <= i; j++){
            MLET(correl_,i,j) = MGET(covar_,i,j) / GET(vol_,i) / GET(vol_,j);
        }
    }
    for(int i = 0; i < correl_->m; i++){
        for(int j = 0; j < i; j++){
            MLET(correl_,j,i) = MGET(correl_,i,j);
        }
    }

}