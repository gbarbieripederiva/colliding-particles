#ifndef __PARTICLE_COLLIDE_EVENT_H__
#define __PARTICLE_COLLIDE_EVENT_H__

#include <iostream>
#include <vector>
#include "../particle/particle.h"

namespace ParticleCollideEventConstants{
    enum EventWall{
        NO_WALL,WALL_X, WALL_Y
    };
};

class ParticleCollideEvent
{
private:
    Particle *p1 = NULL;
    Particle *p2 = NULL;
    double t;
    ParticleCollideEventConstants::EventWall wall = ParticleCollideEventConstants::NO_WALL;
    bool noEvent = false;
public:
    // Constructors
    ParticleCollideEvent(){};
    ParticleCollideEvent(double time,Particle* p, ParticleCollideEventConstants::EventWall wall)
    :t(time),p1(p),wall(wall){};
    ParticleCollideEvent(double time,Particle* p1,Particle* p2)
    :t(time),p1(p1),p2(p2){};

    // Getters
    Particle* getP1() const {return this->p1;};
    Particle* getP2() const {return this->p2;};
    bool getIsWall() const {return this->wall != ParticleCollideEventConstants::NO_WALL;};
    ParticleCollideEventConstants::EventWall getWall() const {return this->wall;};
    double getTime() const {return this->t;};
    bool getNoEvent() const {return this->noEvent;};

    // Setters
    void setP1(Particle* p1){this->p1 = p1;};
    void setP2(Particle* p2){this->p2 = p2;};
    void setWall(ParticleCollideEventConstants::EventWall wall){this->wall = wall;};
    void setTime(double time){this->t = time;};
    void setNoEvent(bool noEvent){this->noEvent = noEvent;};
    
    struct TimeComparator
    {
        bool operator()(const ParticleCollideEvent &p1,const ParticleCollideEvent &p2){
            return p1.t < p2.t;
        }
    };

};

std::ostream &operator<<(std::ostream &os, ParticleCollideEvent const &pce);

#endif