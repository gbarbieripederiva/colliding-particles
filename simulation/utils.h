#ifndef __MY_UTILS_H__
#define __MY_UTILS_H__
#include <vector>
#include <cstdlib>
#include <ctime>
#include <numeric>
#include <queue>
#include "particle/particle.h"
#include "particleCollideEvent/particleCollideEvent.h"
#include "simData.h"

double randomDouble(double min = 0, double max = 1)
{
    return ((double)rand() / RAND_MAX) * (max - min) + min;
}

std::vector<Particle *> generateParticles(int qty,const SimData &simData, double minMass = 0, double maxMass = 0, double minRadius = 0, double maxRadius = 0, double minVel = 0, double maxVel = 0, long seed = -1)
{
    auto vp = std::vector<Particle *>();

    unsigned int realSeed = 0;
    if (seed < 0)
    {
        realSeed = (unsigned)std::time(NULL);
    }
    else
    {
        realSeed = seed % UINTMAX_MAX;
    }
    srand(realSeed);

    for (int i = 0; i < qty; i++)
    {
        bool added = false;
        for (int retries = 0; retries < 10 && !added; retries++)
        {
            Particle p = Particle(i,
                                  randomDouble(minMass, maxMass),
                                  randomDouble(minRadius, maxRadius),
                                  randomDouble(0, simData.simSideX), randomDouble(0, simData.simSideY),
                                  randomDouble(minVel, maxVel), randomDouble(minVel, maxVel));
            bool superimposed = false;
            for (int j = 0; j < vp.size() && !superimposed; j++)
            {
                if (pow(p.distanceTo(*(vp[j])), 2) <= pow(vp[j]->getRadius() + p.getRadius(), 2))
                {
                    superimposed = true;
                }
            }
            if (!superimposed)
            {
                vp.push_back(new Particle(p));
                added = true;
            }
        }
    }
    return vp;
}

void initEventVector(
    const SimData &simData,
    std::vector<Particle *> &particles,
    std::priority_queue<
        ParticleCollideEvent,
        std::vector<ParticleCollideEvent>,
        ParticleCollideEvent::TimeComparator> &events)
{
    for (int i = 0; i < particles.size(); i++)
    {
        for (int j = i; j < particles.size(); j++)
        {
            if (i == j)
            {
                double tx = particles[i]->collideX(0, simData.simSideX);
                if (tx >= 0)
                {
                    events.push(ParticleCollideEvent(tx, particles[i],ParticleCollideEventConstants::WALL_X));
                }
                double ty = particles[i]->collideX(0, simData.simSideY);
                if (ty >= 0)
                {
                    events.push(ParticleCollideEvent(ty, particles[i],ParticleCollideEventConstants::WALL_Y));
                }
            }
            else
            {
                double t = particles[i]->collide(*(particles[j]));
                if(t >= 0){
                    events.push(ParticleCollideEvent(t,particles[i],particles[j]));
                }
            }
        }
    }
}

ParticleCollideEvent advanceEvents(
    const SimData &simData,
    std::vector<Particle *> &particles,
    std::priority_queue<
        ParticleCollideEvent,
        std::vector<ParticleCollideEvent>,
        ParticleCollideEvent::TimeComparator> &events,
        double lastEventTime){
    if(events.empty()){
        return ParticleCollideEvent();
    }
    ParticleCollideEvent p = ParticleCollideEvent(events.top());
    if(!p.getNoEvent()){
        for (int i = 0; i < particles.size(); i++)
        {
            particles[i]->advanceWithNoCollision(p.getTime() - lastEventTime);
        }
        if(p.getIsWall()){
            if(p.getWall()==ParticleCollideEventConstants::WALL_X){
                p.getP1()->bounceX();
            }else{
                p.getP1()->bounceY();
            }
            
        } else{
            p.getP1()->bounce(*p.getP2());
        }
    }

    // else if p is no event return that
    return p;
}

void printState(std::vector<Particle *> &particles, std::ostream &os = std::cout)
{
    os << particles.size() << std::endl;
    for (Particle* p : particles)
    {
        os  << p->getX() << " " 
            << p->getY() << " " 
            << p->getDX() << " " 
            << p->getDY() << " "
            << p->getMass() << " "
            << p->getRadius() << " "
            << 0 << " "
            << 0 << " "
            << 0 << " "<< std::endl;
    }
}

void printState(std::vector<Particle> &particles, std::ostream &os = std::cout)
{
    os << particles.size() << std::endl;
    for (Particle p : particles)
    {
        os  << p.getX() << " " 
            << p.getY() << " " 
            << p.getDX() << " " 
            << p.getDY() << " "
            << p.getMass() << " "
            << p.getRadius() << " "
            << 0 << " "
            << 0 << " "
            << 0 << " "<< std::endl;
    }
}

std::vector<Particle *> getParticles(std::istream &f)
{
    int particles = 0;
    f >> particles;
    std::vector<Particle *> vec;
    int id = 0;
    double x, y, dx, dy, mass, radius;
    int r,g,b;
    while (f >> x >> y >> dx >> dy >> mass >> radius >> r >> g >> b)
    {
        vec.push_back(new Particle(id, mass, radius, x, y, dx, dy));
        id++;
    }
    return vec;
}

#endif