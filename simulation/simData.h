#ifndef __SIM_DATA_H__
#define __SIM_DATA_H__

class SimData
{
public:
    double simSideX = 0;
    double simSideY = 0;
    int eventsToProcess = 0;
    SimData(){};
    SimData(double simSideX, double simSideY) : simSideX(simSideX), simSideY(simSideY){};
    SimData(double simSideX, double simSideY, int eventsToProcess)
        : simSideX(simSideX), simSideY(simSideY),eventsToProcess(eventsToProcess){};
};

#endif