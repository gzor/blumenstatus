#include "plant.h"
#include "pindefintion.h"

#ifdef MOISTPIN1
    Plant plant1(MOISTPIN1,relayPin1);
#endif
#ifdef MOISTPIN1
    Plant plant1(MOISTPIN2,relayPin2);
#endif
#ifdef MOISTPIN1
    Plant plant1(MOISTPIN3,relayPin3);
#endif
#ifdef MOISTPIN1
    Plant plant1(MOISTPIN4,relayPin4);
#endif

void initAllPlants()
{
    #ifdef MOISTPIN1
        plant1.init();
    #endif
    #ifdef MOISTPIN2
        plant2.init();
    #endif
    #ifdef MOISTPIN3
        plant3.init();
    #endif
    #ifdef MOISTPIN4
        plant4.init();
    #endif
}
int* handleAllAvailablePlants()
{
    int moistureValues[5] = {-1};
    #ifdef MOISTPIN1
        moistureValues[0] = plant1.handle();
    #endif
    #ifdef MOISTPIN2
        moistureValues[1] = plant2.handle();
    #endif
    #ifdef MOISTPIN3
        moistureValues[2] = plant3.handle();
    #endif
    #ifdef MOISTPIN4
        moistureValues[3] = plant4.handle();
    #endif
    return moistureValues;
}