#ifndef __PARTICLE_COLLIDE_EVENT_H__
#define __PARTICLE_COLLIDE_EVENT_H__

#include <iostream>
#include <vector>
#include "../particle/particle.h"

enum Wall{
    WALL_X_POSITIVO,
    WALL_Y_POSITIVO,
    WALL_X_NEGATIVO,
    WALL_Y_NEGATIVO
};

class ParticleCollideEvent
{
private:
    Particle p1;
    Particle p2;
    double t;
    Wall w;
    bool isWall = false;
    bool noEvent = false;
public:
    // Constructors
    ParticleCollideEvent();
    ParticleCollideEvent(Particle p, Wall w, double time);
    ParticleCollideEvent(Particle p1,Particle p2, double time);

    // Getters
    Particle getP1() const;
    Particle getP2() const;
    Wall getWall() const;
    bool getIsWall() const;
    double getTime() const;
    bool getNoEvent() const;

    // Setters
    void setP1(Particle p1);
    void setP2(Particle p2);
    void setWall(Wall wall);
    void setIsWall(bool isWall);
    void setTime(double t);
    void setNoEvent(bool noEvent);

    std::vector<double> newVelsAfterEvent() const;
};

std::ostream &operator<<(std::ostream &os, ParticleCollideEvent const &pce);

#endif