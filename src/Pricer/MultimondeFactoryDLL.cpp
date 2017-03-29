#include "MultimondeFactoryDLL.hpp"
#include "MultimondeFactory.hpp"

void MultimondeFactory::PriceDLL(double t, int year, int month, int day, double &price, double &std, char *pathDatas) {
    MultimondeFactory::Price(t,year,month,day,price,std,pathDatas);
}

void MultimondeFactory::HedgeDLL(double t, int year, int month, int day, double *compo, double *std, char *pathDatas) {
    MultimondeFactory::Hedge(t,year,month,day,compo,std,pathDatas);
}

void MultimondeFactory::BackTestDLL(int hedgingNb, int MCnb, char *path, char *pathDatas, double discrStep) {
    MultimondeFactory::BackTest(hedgingNb,MCnb,path,pathDatas,discrStep);
}

void MultimondeFactory::ForwardTestDLL(int hedgingNb, int MCnb, char *path, char *pathDatas, double discrStep) {
    MultimondeFactory::ForwardTest(hedgingNb, MCnb, path, pathDatas, discrStep);
}
