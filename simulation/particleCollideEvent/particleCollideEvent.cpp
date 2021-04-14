#include "particleCollideEvent.h"

std::ostream &operator<<(std::ostream &os, ParticleCollideEvent const &pce)
{
    if(pce.getNoEvent()){
        return os   << "{ noEvent:true }";
    }
    else if(pce.getIsWall()){
        return os   << "{ p:" << *pce.getP1()
                    << ", time:" << pce.getTime()
                    << " }";
    }else{
        return os   << "{ p1:" << *pce.getP1()
                    << ", p2:" << *pce.getP2()
                    << ", time:" << pce.getTime()
                    << " }";
    }
}
