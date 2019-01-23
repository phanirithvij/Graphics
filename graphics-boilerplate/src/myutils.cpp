#include "myutils.h"
#include "bits/stdc++.h"

int64_t getEpochTime(){
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();
}


bool pnpoly(int nvert, float *vertx, float *verty, float testx, float testy){
    /* see: https://stackoverflow.com/a/2922778/8608146 */
    int i, j;
    bool c = false;
    for (i = 0, j = nvert-1; i < nvert; j = i++) {
        if ( ((verty[i]>testy) != (verty[j]>testy)) &&
       (testx < (vertx[j]-vertx[i]) * (testy-verty[i]) / (verty[j]-verty[i]) + vertx[i]) )
           c = !c;
    }
    return c;
}


double fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}
