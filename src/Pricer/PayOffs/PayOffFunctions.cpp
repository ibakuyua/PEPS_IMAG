#include "PayOffFunctions.hpp"

#include "../infoMultimonde.hpp"
double payOffMultimonde21(PnlMat *path, PnlVect *parameters) {
    int nbTimeStep = path->m;
    bool isConstated[6] = {false, false, false, false,false,false};
    int constatationIndexes[6] =
            {
                    (int)(nbTimeStep * NB_DAYS_TO_CONSTATATION_1/(double)NB_DAYS_TO_MATURITY),
                    (int)(nbTimeStep * NB_DAYS_TO_CONSTATATION_2/(double)NB_DAYS_TO_MATURITY),
                    (int)(nbTimeStep * NB_DAYS_TO_CONSTATATION_3/(double)NB_DAYS_TO_MATURITY),
                    (int)(nbTimeStep * NB_DAYS_TO_CONSTATATION_4/(double)NB_DAYS_TO_MATURITY),
                    (int)(nbTimeStep * NB_DAYS_TO_CONSTATATION_5/(double)NB_DAYS_TO_MATURITY),
                    (int)(nbTimeStep * NB_DAYS_TO_CONSTATATION_6/(double)NB_DAYS_TO_MATURITY)
            };

    double maxPerfCour, indexPerf, constatedPerf = 0.;
    int maxPerfCourIndex;

    // For each date of constatation
    for (int d = 0; d < 6; ++d) {
        maxPerfCour = -10000;
        // For each index performance
        for (int ind = 0; ind < 6; ++ind) {
            // Non constated index
            if (isConstated[ind])
                continue;
            // Compute the index performance
            indexPerf = (PNL_MGET(path,constatationIndexes[d],ind)-PNL_MGET(path,0,ind))/PNL_MGET(path,0,ind);
            // Search of the maximum
            if (maxPerfCour < indexPerf){
                maxPerfCour = indexPerf;
                maxPerfCourIndex = ind;
            }
        }
        // constated index is deleted of the index basket
        isConstated[maxPerfCourIndex] = true;
        // cap/floor the maxPerf
        maxPerfCour = (maxPerfCour < FLOOR) ? FLOOR : maxPerfCour;
        maxPerfCour = (maxPerfCour > CAP) ? CAP : maxPerfCour;
        // Add the performance to the final performance
        constatedPerf += maxPerfCour;
    }

    return NOMINAL * (1 + constatedPerf);
}

double payOffCall(PnlMat *path, PnlVect *parameters) {
    double S_TminusK = MGET(path,path->m-1,0)-GET(parameters,0);

    return (S_TminusK > 0.) ? S_TminusK : 0.;
}
